#include "scenes/MenuScene.h"

#include "Camera.h"
#include "Constants.h"
#include "ECS/Components.h"
#include "ECS/ECS.h"
#include "Input.h"
#include "TextureManager.h"
#include "systems/Systems.h"

#include <SDL2/SDL.h>

#include <memory>

MenuScene::MenuScene() {
   Map::loadBlockIDS();

   TextureManager::Get().SetBackgroundColor(BackgroundColor::BLUE);

   Camera::Get().setCameraX(0);
   Camera::Get().setCameraY(0);

   menuSystem = world->registerSystem<MenuSystem>();
   world->registerSystem<RenderSystem>();

   createMenuEntities();
}

void MenuScene::handleInput() {
   world->handleInput();

   if (world->hasSystem<OptionsSystem>()) {
      if (world->getSystem<OptionsSystem>()->isFinished()) {
         world->unregisterSystem<OptionsSystem>();
         menuSystem->setEnabled(true);
         menuSystem->showMenuText();
      }
      return;
   }

   if (Input::Get().getKeyPressed(Key::MENU_ACCEPT)) {
      if (menuSystem->levelSelected()) {
         finished = true;
      } else if (menuSystem->optionsSelected()) {
         world->registerSystem<OptionsSystem>();
         menuSystem->setEnabled(false);
         menuSystem->hideMenuText();
      }
      return;
   }
}

bool MenuScene::isFinished() {
   return finished;
}

int MenuScene::getSelectedLevel() {
   return menuSystem->getSelectedLevel();
}

int MenuScene::getSelectedSublevel() {
   return menuSystem->getSelectedSublevel();
}

void MenuScene::createMenuEntities() {
   std::shared_ptr<SDL_Texture> backgroundTexture =
       TextureManager::Get().LoadSharedTexture("res/sprites/blocks/background.png");

   Entity* entity(world->create());

   entity->addComponent<PositionComponent>(
       Vector2f(0, 0),
       Vector2i(SCREEN_WIDTH, SCREEN_HEIGHT));

   entity->addComponent<TextureComponent>(backgroundTexture, false, false);
   entity->addComponent<BackgroundComponent>();
}
