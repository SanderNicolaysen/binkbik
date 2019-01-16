#ifndef BINKBIK_PLANETVIEW_H
#define BINKBIK_PLANETVIEW_H


#include "NestedState.h"

class PlanetView : public NestedState
{
public:
    PlanetView(NestedMachine &nestedContext, Machine &context, sf::RenderWindow &window, int planetID);

    void enter() override;

    void run() override;

    void exit() override;

protected:
    sf::Texture texture;
    sf::Sprite background;

    sf::Font font;
    sf::Text text;

    SoundEffect* soundEffect = SoundEffect::Instance();

    std::string planetTexts[13] = {
            "PLANET Baltia\n\n"
            "The planet of Baltia houses 5 billion human\n"
            "lifeforms. Baltia is the trade capital of \n"
            "this solar system. Here, all types of \n"
            "species can trade with eachother which in \n"
            "turn benefits the hard working asteroid \n"
            "miners. The planet welcomes you!",

            "Argus\n\n"
            "The planet of Argus has a population of over\n"
            "7 billion Draenei, an alien lifeform. This\n"
            "civilization is highly religious, and is usually\n"
            "hostile towards newcomers. Be aware!\n",

            "Steambang\n\n"
            "Homeplanet of the humans in this solar system.\n"
            "This is where you were born, and where your family\n"
            "lives. The planet welcomes you home, and grants you\n"
            "a full tank of fuel.\n",

            "Robotopia\n\n"
            "Robotopia is the first planet to be controlled\n"
            "100% by AI. They are the best technical\n"
            "engineers in the galaxy. Do not joke about\n"
            "robots taking over here, they do not find it to\n"
            "be funny.\n",

            "Yeknod\n\n"
            "Home of the insect species. There are\n"
            "Human colonies scattered about this planet\n"
            "and that is where most of the trade is done.\n"
            "The local life is primitive and hostile. Do not\n"
            "engage.\n",

            "Nior\n\n"
            "On this planet, androids and humans live\n"
            "together in a strange utopian society. This planet\n"
            "is the military super power in the galaxy, and its\n"
            "generally a good idea to stay away from this planet\n"
            "unless you are invited.\n",

            "X435X43\n\n"
            "Colony planet.\n"
            "Population: Human, Android, Yeknodians\n"
            "Trade: Minerals\n",

            "X324X34\n\n"
            "Mining planet.\n"
            "Population: Android\n"
            "Trade: Minerals, natural gas\n",

            "X324X35\n\n"
            "Mining planet.\n"
            "Population: Yeknodian\n"
            "Trade: Minerals, natural gas\n",

            "Safari\n\n"
            "Safari planet.\n"
            "Population: Human, earth creatures\n"
            "Trade: Entertainment\n",

            "Alderaan\n\n"
            "Self proclaimed monarchy. Happy\n"
            "Jarjars.\n"
            "Population: Human and Gungan\n"
            "Trade: Minerals, natural gas\n",

            "Tellus434\n\n"
            "Colony planet\n"
            "Population: Human, Draenei\n"
            "Function: Education\n",

            "Etirps\n\n"
            "Rock planet, unwelcome atmosphere.\n"
            "Abandoned Draenei colony.\n"
    };
};


#endif //BINKBIK_PLANETVIEW_H
