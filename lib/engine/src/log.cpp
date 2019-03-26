#include <iostream>
#include "engine/engine.hpp"
#include "engine/log.hpp"

/**
 * Fonction: Engine::Log::info
 * -------------------
 * Affiche dans le terminal une information.
 *
 * @param text Texte à afficher dans le terminal
 */
void Engine::Log::info(const char text[100])
{
  std::cout << "[]" << "[info] " << text << std::endl;
}

/**
 * Fonction: Engine::Log::info
 * -------------------
 * Affiche dans le terminal un message d'erreur.
 *
 * @param text Message d'erreur à afficher dans le terminal
 */
void Engine::Log::error(const char text[100])
{
  std::cout << "[ERROR] " << text << std::endl;
}
