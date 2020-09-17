
#include <iostream>

#include "Subject.h"
#include "Reporter.h"
#include "PCDisplay.h"
#include "SmartphoneDisplay.h"

#define LINE_SEPERATOR std::cout << "\n--------------------------------------------------------------------\n" << std::endl

int main()
{
    Subject<NewspaperData> newsMediator;

    Reporter wallStreetJournal;
    Reporter newYorkTimes;

    PCDisplay myComputer;
    SmartphoneDisplay galaxyS7;

    wallStreetJournal.
    SetNewspaperName("U.S. Retail Spending Grew at Slower Pace in August").
    SetAuthor("Harriet Torry").
    SetCreatedDate("Sept. 16, 2020").
    SetNewspaperBody("U.S. retail spending rose 0.6% in August for the fourth straight monthly increase, "
        "but at a slower pace as some extra unemployment benefits ran out.\n\n"
        "The increase reported by the Commerce Department marks an eased pace than earlier "
        "in the summer when spending rebounded sharply from steep declines that occurred early "
        "in the coronavirus pandemic. July's gain was revised lower, "
        "to a 0.9% increase from an earlier reading of a 1.2% rise.\n\n"
        "Consumers spent more on clothing, electronics and furniture in August as "
        "students went back to school, many for online classes because of the pandemic. "
        "Spending fell on groceries and sporting goods.");

    newYorkTimes.
    SetNewspaperName("Some of the Planet's Most Polluted Skies Are Now Over the West Coast").
    SetAuthor("Mike Baker").
    SetCreatedDate("Sept. 15, 2020").
    SetNewspaperBody("SEATTLE - The billowing wildfire smoke that has blanketed much of the "
        "West Coast with a caustic haze also began settling into the "
        "atmosphere thousands of miles away on Tuesday.\n\n"
        "While more favorable weather has allowed firefighters to make some progress against "
        "the devastating blazes in Oregon, the crisis was far from over: "
        "The largest fires were still mostly uncontained, the air has been some of "
        "the most polluted on the planet for a week, and the state was setting up a mobile "
        "morgue as crews continued to sift through the rubble for missing people.\n\n"
        "In California, where 25 people have died this year and 3.2 million acres "
        "- a modern record - have burned, officials had both successes and setbacks: "
        "Firefighters there contained two fires, one in Yuba County and the other north of Willits, "
        "and were trying to suppress 33 new blazes as stiff winds in the northeast "
        "pushed fires into new territory. There was still no significant rain in sight, "
        "and lands parched by warm weather remain at risk of igniting.");

    newsMediator.Subscribe(&myComputer);
    newsMediator.Subscribe(&galaxyS7);


    newsMediator.Publish(wallStreetJournal.GetNewspaperData());

    myComputer.Display();
    LINE_SEPERATOR;
    galaxyS7.Display();
    LINE_SEPERATOR;


    newsMediator.Publish(newYorkTimes.GetNewspaperData());

    myComputer.Display();
    LINE_SEPERATOR;
    galaxyS7.Display();
    LINE_SEPERATOR;

    return 0;
}
