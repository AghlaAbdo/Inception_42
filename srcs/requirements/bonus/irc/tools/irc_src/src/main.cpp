# include "../inc/Server.hpp"
# include "../inc/Client.hpp"
# include "../inc/utils.hpp"

int Server::terminate = 0;

int main(int argc, char **argv)
{
    if (argc != 3)
        return (std::cerr << "Invalid number of arguments: <port> <password>\n", 1);
    try{
        int                 port;
        if (!validPort(argv[1]))
            return (std::cerr << "Not valid port!\n", 1);
        std::stringstream   ss(argv[1]);
        ss >> port;
        if (port > 65535 || port < 1)
            return (std::cerr << "Not valid port!\n", 1);
        if (!validPass(argv[2]))
            return (std::cerr << "Invalid password for the server!\n", 1);
        signal(SIGINT, Server::signalHandler);
        signal(SIGQUIT, Server::signalHandler);
        Server server = Server(port, argv[2]);
        server.listening();
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
