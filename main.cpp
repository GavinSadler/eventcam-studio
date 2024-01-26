
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <metavision/hal/facilities/i_event_decoder.h>
#include <metavision/sdk/base/events/event_cd.h>
#include <metavision/sdk/base/events/event_ext_trigger.h>
#include <metavision/sdk/core/algorithms/periodic_frame_generation_algorithm.h>
#include <metavision/sdk/driver/camera.h>


using std::string;
using std::cout, std::cerr, std::endl;

int main(int argc, char** argv)
{

#ifdef _DEBUG
    string in_raw_file_path = "../../../recordings/dan402_2_extTrig.raw";
#else

    if (argc < 2)
    {
        std::cerr << "Error: input filepath needed.\n";
        exit(1);
    }

    string in_raw_file_path = argv[1];
#endif

    Metavision::Camera camera = Metavision::Camera::from_file(in_raw_file_path);

    //Metavision::timestamp min(LLONG_MAX), max(LLONG_MIN);
    //std::cout << min << ";" << max << std::endl;
    //
    //auto cbid = camera.cd().add_callback(
    //    [&](const Metavision::EventCD* b, const Metavision::EventCD* e) {
    //        if (b->t < min)
    //            min = b->t;
    //        if (e->t > max)
    //            max = e->t;
    //        //std::cout << b->t << "\t" << e->t << std::endl;
    //    }
    //);

    int eventCount = 0;
    auto events = std::vector<const Metavision::EventExtTrigger*>();
    std::set<Metavision::EventExtTrigger> eventsSet;
    std::set<int> eventsTimestampSet;

    camera.get_device().get_facility<Metavision::I_EventDecoder<Metavision::EventExtTrigger>>()->add_event_buffer_callback(
        [&](const Metavision::EventExtTrigger* begin, const Metavision::EventExtTrigger* end)
        {
            for (auto i = begin; i < end; i++)
            {
                events.push_back(i);
                eventsSet.insert(Metavision::EventExtTrigger(*i));
                eventsTimestampSet.insert(i->t);
                eventCount++;
            }
        }
    );

    // Despite looking exactly like the code above, this method of adding a callback has significant overhead
    // Like, it takes over 100X the amount of time to do this instead of the callback defined above
    /*
    auto extTrig = camera.ext_trigger().add_callback(
        [&events](const Metavision::EventExtTrigger* begin, const Metavision::EventExtTrigger* end)
        {
            events++;
            //std::cout << *begin << "\t" << *end << std::endl;
        }
    );
    */

    camera.start();

    std::cout << "Resolution: " << camera.geometry().width() << "x" << camera.geometry().height() << std::endl;

    while (camera.is_running()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 1));
        std::cout << "Events so far: " << eventCount << std::endl;
    }

    camera.stop();

    std::cout
        << "Events vector count: " << events.size()
        << "\tEvents set count: " << eventsSet.size()
        << "\tEvents timestamp set count" << eventsTimestampSet.size()
        << "\tRising and falling edge pairs: " << events.size() / 2 << std::endl;

    /*for (auto& e : eventsSet)
    {
        std::cout << e << "\n";
    }*/

    return 0;
}
