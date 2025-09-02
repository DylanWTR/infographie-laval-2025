#pragma once

#include <imgui.h>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "application/menu/lateral-menu.hpp"
#include "pipeline/primitives/primitive.hpp"
#include "window/window.hpp"

struct Folder;

using FolderPtr = std::shared_ptr<Folder>;
using PrimitivePtr = std::shared_ptr<Primitive>;

struct Folder {
    std::string name;
    FolderPtr parent;
    std::vector<FolderPtr> subfolders;
    std::vector<PrimitivePtr> primitives;

    Folder(const std::string &name, FolderPtr parent = nullptr);
};

class GraphMenu final : public LowerLateralMenu {
    Window &_window;
    std::vector<PrimitivePtr> &_primitiveList;
    FolderPtr _rootFolder;
    FolderPtr _folderToRename;
    FolderPtr _folderToDelete;
    FolderPtr _selectedFolder;
    char _renameBuffer[256];

    void renderFolder(FolderPtr folder, PrimitivePtr &selectedPrimitive);
    void renderFolderSelectionPopup(FolderPtr folder, PrimitivePtr primitive);
    void movePrimitiveToFolder(PrimitivePtr primitive, FolderPtr targetFolder);
    std::vector<FolderPtr> getAllFolders(FolderPtr folder);
    void updatePrimitives();

public:
    explicit GraphMenu(Window &_window, std::vector<PrimitivePtr> &primitiveList);
    void renderMenu(PrimitivePtr &selectedPrimitive) override;

    void createFolder(const std::string &name, FolderPtr parent);
    void renameFolder(FolderPtr folder);
    void deleteFolder(FolderPtr folder);
};

using GraphMenuPtr = std::unique_ptr<GraphMenu>;
