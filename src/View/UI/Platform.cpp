#include <View/UI/Platform.h>
#include <ViewModel/GameViewModel.h>

using View::UI::Platform;

void Platform::init(std::shared_ptr<ViewModel::GameViewModel> view_model,int id) {
    this->id = id;
    size = sf::Vector2f(200, 30);

    switch (view_model->getPlatformTypeById(id)) {
        case View::UI::PlatformType::NORMAL:
            break;
    }

    rect.setPosition(view_model->getPlatformPositionById(id));
    rect.setSize(size);
}

void Platform::render(sf::RenderWindow& window){
    window.draw(rect);
}
