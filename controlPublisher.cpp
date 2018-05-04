#include <string>
#include <iostream>
#include <fstream>

#include <zmq.hpp>
#include "zhelpers.hpp"

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

using namespace std;

int main(int argc, char *argv[]) { // enter device as argument to change device
    const int BUFFER_SIZE = 1024;

    // Prepare context, publisher and topic
    zmq::context_t context(2);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.connect("tcp://localhost:5556");
    std::cout << "server started" << std::endl;
    const char *device = (argc > 1) ? argv[1] : "/dev/rfcomm0";
    const char *topic = "controller";

    boost::asio::io_service io;
    boost::asio::serial_port port(io, device);
    port.set_option(boost::asio::serial_port_base::baud_rate(9600)); // Default for bluetooth
    port.set_option(boost::asio::serial_port_base::character_size(8));

    while (true) {
        char c;
        auto *buffer = new char[BUFFER_SIZE];
        int i = 0;

        while (boost::asio::read(port, boost::asio::buffer(&c, 1)) == 1) {
            buffer[i++] = c;

            if (c == '}') {
                break; // End of command
            }
        }

        std::string command(buffer);
        std::size_t separator_pos = command.find(':');

        std::string control = command.substr(1, separator_pos - 1); // Substring between '{' and ':'
        std::string state = command.substr(separator_pos + 1,
                                           command.length() - separator_pos - 2); // Substring between ':' and '}'

        cout << "command: " << command << endl << "control: " << control << endl << "state: " << state << endl;

        // Send topic
        s_sendmore(publisher, topic);

        // Send control button/joystick
        s_sendmore(publisher, control);

        // Send message
        s_send(publisher, state);
    }
    port.close();

    return 0;
}
