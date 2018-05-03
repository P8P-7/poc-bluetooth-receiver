#include <zmq.hpp>
#include "zhelpers.hpp"

int main ()
{
    // Prepare context, subscriber and topic
    zmq::context_t context (2);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5555");

    //  Subscribe to topic
    const char *filter = "controller";
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));

    // interupt helper function

    s_catch_signals ();

    while (1) {
        try {
            //  Read envelope with address
            std::string address = s_recv(subscriber);
            //  Read control button/joystick
            std::string control = s_recv(subscriber);
            //  Read control state
            std::string state = s_recv(subscriber);

            std::cout << "[" << control << "]\t " << state << std::endl;
        }
        catch(zmq::error_t& e) {
            std::cout << "W: interrupt received, proceeding…" << std::endl;
        }
        if (s_interrupted) {
            std::cout << "W: interrupt received, killing subscriber…" << std::endl;
            break;
        }
    }
}
