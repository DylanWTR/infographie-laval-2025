#include "application/menu/graph-menu.hpp"

Folder::Folder(const std::string &name, FolderPtr parent)
    : name(name), parent(parent) {}

GraphMenu::GraphMenu(Window &window, std::vector<PrimitivePtr> &primitiveList) :
    LowerLateralMenu("Graph Scene"), _window(window), _primitiveList(primitiveList), _folderToRename(nullptr), _selectedFolder(nullptr) {
    _rootFolder = std::make_shared<Folder>("Root");
    _selectedFolder = _rootFolder;
    memset(_renameBuffer, 0, sizeof(_renameBuffer));
}

void GraphMenu::renderMenu(PrimitivePtr &selectedPrimitive) {
    updatePrimitives();
    renderFolder(_rootFolder, selectedPrimitive);

    if (_folderToRename) {
        ImGui::OpenPopup("Rename Folder");
        if (ImGui::BeginPopupModal("Rename Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::InputText("New Name", _renameBuffer, sizeof(_renameBuffer));
            if (ImGui::Button("Rename")) {
                renameFolder(_folderToRename);
                _folderToRename = nullptr;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                _folderToRename = nullptr;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}

void GraphMenu::renderFolder(FolderPtr folder, PrimitivePtr &selectedPrimitive) {
    if (ImGui::TreeNode(folder->name.c_str())) {
        if (ImGui::IsItemClicked()) {
            _selectedFolder = folder;
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::Selectable("Create Subfolder")) {
                createFolder("New Subfolder", folder);
            }
            if (ImGui::Selectable("Rename")) {
                _folderToRename = folder;
                strncpy(_renameBuffer, folder->name.c_str(), sizeof(_renameBuffer));
            }
            if (ImGui::Selectable("Delete")) {
                _folderToDelete = folder;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        for (auto &subfolder : folder->subfolders) {
            renderFolder(subfolder, selectedPrimitive);
        }

        int i = 0;
        for (const auto &primitive : folder->primitives) {
            bool isSelected = (selectedPrimitive == primitive);
            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
            }

            ImGui::Button((std::string("Primitive ") + std::to_string(i)).c_str());

            if (ImGui::IsItemHovered())
                _window.setCursor(CursorType::HAND);

            if (ImGui::IsItemClicked()) {
                selectedPrimitive = primitive;
            }

            ImGui::SameLine();
            if (ImGui::Button(("Move##" + std::to_string(i)).c_str())) {
                ImGui::OpenPopup(("MovePrimitivePopup##" + std::to_string(i)).c_str());
            }

            if (ImGui::IsItemHovered())
                _window.setCursor(CursorType::HAND);

            if (ImGui::BeginPopup(("MovePrimitivePopup##" + std::to_string(i)).c_str())) {
                renderFolderSelectionPopup(_rootFolder, primitive);
                ImGui::EndPopup();
            }

            if (isSelected) {
                ImGui::PopStyleColor();
            }
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Click to select, use Move button to move");
            }

            i++;
        }

        ImGui::TreePop();
    }

    if (_folderToDelete) {
        deleteFolder(_folderToDelete);
        _folderToDelete = nullptr;
    }
}

void GraphMenu::renderFolderSelectionPopup(FolderPtr folder, PrimitivePtr primitive) {
    if (ImGui::Selectable(folder->name.c_str())) {
        movePrimitiveToFolder(primitive, folder);
    }

    for (auto &subfolder : folder->subfolders) {
        renderFolderSelectionPopup(subfolder, primitive);
    }
}

void GraphMenu::createFolder(const std::string &name, FolderPtr parent) {
    auto newFolder = std::make_shared<Folder>(name, parent);
    parent->subfolders.push_back(newFolder);
}

void GraphMenu::renameFolder(FolderPtr folder) {
    if (!folder || strlen(_renameBuffer) == 0) return;
    folder->name = _renameBuffer;
}

void GraphMenu::deleteFolder(FolderPtr folder) {
    if (folder->parent) {
        for (auto &primitive : folder->primitives) {
            folder->parent->primitives.push_back(primitive);
        }

        auto &subfolders = folder->parent->subfolders;
        subfolders.erase(std::remove(subfolders.begin(), subfolders.end(), folder), subfolders.end());
    } else {
        folder->primitives.clear();
    }
}

void GraphMenu::movePrimitiveToFolder(PrimitivePtr primitive, FolderPtr targetFolder) {
    for (auto &folder : getAllFolders(_rootFolder)) {
        auto &primitives = folder->primitives;
        primitives.erase(std::remove(primitives.begin(), primitives.end(), primitive), primitives.end());
    }

    targetFolder->primitives.push_back(primitive);
}

std::vector<FolderPtr> GraphMenu::getAllFolders(FolderPtr folder) {
    std::vector<FolderPtr> folders;
    folders.push_back(folder);

    for (auto &subfolder : folder->subfolders) {
        auto subfolders = getAllFolders(subfolder);
        folders.insert(folders.end(), subfolders.begin(), subfolders.end());
    }

    return folders;
}

void GraphMenu::updatePrimitives() {
    for (const auto &primitive : _primitiveList) {
        bool found = false;
        for (const auto &folder : getAllFolders(_rootFolder)) {
            if (std::find(folder->primitives.begin(), folder->primitives.end(), primitive) != folder->primitives.end()) {
                found = true;
                break;
            }
        }
        if (!found) {
            _rootFolder->primitives.push_back(primitive);
        }
    }
}