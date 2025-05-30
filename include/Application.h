#ifndef APPLICATION_H
#define APPLICATION_H

#include "CommandParser.h"
#include "Component.h"

#define MAX_COMPONENTS 10

class Application {
private:
    Component* m_components[MAX_COMPONENTS]{};
    int m_componentCount;


public:
    Application();

    // Ajouter un composant avec ses mots-clés
    bool addComponent(Component* component);

    // Exécuter une commande
    void run(const CommandParser::ParsedCommand& command) const;
};

#endif // APPLICATION_H