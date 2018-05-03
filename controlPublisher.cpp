#include <SerialStream.h>
#include <string>
#include <iostream>
#include <fstream>
#include <zmq.hpp>
#include "zhelpers.hpp"

using namespace std;
using namespace LibSerial;

int main(int argc, char *argv[]) { // enter device as argument to change device

    // Prepare context, publisher and topic
    zmq::context_t context(2);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.connect("tcp://localhost:5556");
    std::cout << "server started" << std::endl;
    const char *device = (argc > 1)? argv [1]: "/dev/rfcomm0";
    const char *topic = "controller";

    SerialStream stream;

    stream.Open(device);
    stream.SetBaudRate(BaudRate::BAUD_9600); // Default for bluetooth
    stream.SetCharacterSize(CharacterSize::CHAR_SIZE_8);

    std::string control, state;

    while (1) {
        char c;
        stream >> c;
        if (c == '{') {
            stream >> c; // Skip '{'

            while (c != ':') {
                control += c;
                stream >> c;
            }

            stream >> c; // Skip ':'

            while (c != '}') {
                state += c;
                stream >> c;
            }

            cout << "control: " << control << "\t state: " << state << endl;

            // Send topic
            s_sendmore(publisher, topic);
            // Send control button/joystick
            s_sendmore(publisher, control);
            // Send message
            s_send(publisher, state);

            // Reset strings
            control = state = "";
        }
    }

    stream.Close();
    return 0;
}
