#include <imgui.h>
#include <imgui_stdlib.h>

#include "exception/model-exception.hpp"
#include "application/menu/model-menu.hpp"
#include "application/application.hpp"

class Application;

ModelMenu::ModelMenu(Application &application) : FloatingMenu("New Model"), _application(application) {
}

void ModelMenu::renderMenu() {
    ImGui::InputText("Model Path", &_modelPath);

    if (ImGui::Button("Load Model")) {
        try {
            const auto model = std::make_shared<Model>(_modelPath);

            _application.createModel(model);
            _modelPath = "resources/object/";
        } catch (const ModelException &exception) {
            _modelPath = "Unknown model";
            Logger::getInstance().error("{}", exception.what());
        }
    }
}
