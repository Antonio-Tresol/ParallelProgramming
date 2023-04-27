// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <cassert>
#include <stdexcept>
#include <string>

#include "HttpApp.hpp"
#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Socket.hpp"

// TODO(you): Implement connection handlers argument
const char* const usage =
  "Usage: webserv [port] [handlers]\n"
  "\n"
  "  port        Network port to listen incoming HTTP requests, default "
    DEFAULT_PORT "\n"
  "  handlers     Number of connection handler theads\n";

HttpServer::HttpServer() {
}

HttpServer::~HttpServer() {
}

void HttpServer::listenForever(const char* port) {
  return TcpServer::listenForever(port);
}
///---THIS IS WHAT WE HAVE TO PARALLELIZE!---- ///
/*
* main thread should only listen for connections and then send the while part
* to another thread (conecction handler) using a queue of sockets.
* we can hava a array of connection handlers and a queue so that
* they consume from the queue of sockets made by the main thread (httpserver)
* and then they send the response to the client.
* the handlers will produce a queue of requests to the webapps(the ones in
* charge of doing the work) and then the webapps. in this case we only have
* one webapp (goldbachWebApp) so will have one queue going to that webapp only.
* if we had more webapps we would have more queues going to each webapp.
* the web app itself will use multiple threads(second part of the proyect)
* to do the work and then it makes a queue of responses to the will handle by 
* a packing thread(dispatcher/response handler) that will send back the response
* to the client.
* 
*/
//TODO/ everything inside this method needs to be done by the handlers,
// but the method
// stays here
// overrides the TCP server handle connection for its own purpose
void HttpServer::handleClientConnection(Socket& client) {
  // TODO(you): Make this method concurrent. Store client connections (sockets)
  // into a collection (e.g thread-safe queue) and stop

  // TODO(you) Move the following loop to a consumer thread class
  // While the same client asks for HTTP requests in the same connection
  while (true) { /// recieves all the requests from the client
    // Create an object that parses the HTTP request from the socket
    HttpRequest httpRequest(client);

    // If the request is not valid or an error happened
    if (!httpRequest.parse()) { /// get the request from the client
      // Non-valid requests are normal after a previous valid request. Do not
      // close the connection yet, because the valid request may take time to
      // be processed. Just stop waiting for more requests
      break; // if the request is not valid, it will break the loop
    }

    // A complete HTTP client request was received. Create an object for the
    // server responds to that client's request
    HttpResponse httpResponse(client); // generates the response

    // Give subclass a chance to respond the HTTP request
    // this give part to the subclass in charge of responding the request
    const bool handled = this->handleHttpRequest(httpRequest, httpResponse);

    // If subclass did not handle the request or the client used HTTP/1.0
    // this question is do i have to close the connection or not
    if (!handled || httpRequest.getHttpVersion() == "HTTP/1.0") {
      // The socket will not be more used, close the connection
      client.close();
      break;
    }

    // This version handles just one client request per connection
    // TODO(you): Remove this break after parallelizing this method
    break;
  }
}

void HttpServer::chainWebApp(HttpApp* application) {
  assert(application);
  this->applications.push_back(application);
}

int HttpServer::start(int argc, char* argv[]) {
  bool stopApps = false;
  try { //check if in arguments there is a port
    if (this->analyzeArguments(argc, argv)) {
      // Start the log service
      Log::getInstance().start(); // record save in log.txt so
      // we can see what is happening in the server
      // see log.cpp and log.hpp to know more about the log service

      // Start all web applications
      for (size_t index = 0; index < this->applications.size(); ++index) {
        this->applications[index]->start();
      }
      stopApps = true;

      // Start waiting for connections
      this->listenForConnections(this->port);// opening the port to listen for
      const NetworkAddress& address = this->getNetworkAddress();
      Log::append(Log::INFO, "webserver", "Listening on " + address.getIP()
        + " port " + std::to_string(address.getPort()));

      // Accept all client connections
      // risky metaphor: this is like a secretary that answers the phone
      // and does everything the client asks
      // normally task are distributed to other people
      this->acceptAllConnections();
    }
  } catch (const std::runtime_error& error) {
    std::cerr << "error: " << error.what() << std::endl;
  }

  // If applications were started
  if (stopApps) {
    // Stop web applications. Give them an opportunity to clean up
    for (size_t index = 0; index < this->applications.size(); ++index) {
      this->applications[index]->stop();
    }
  }

  // Stop the log service
  Log::getInstance().stop();

  return EXIT_SUCCESS;
}

bool HttpServer::analyzeArguments(int argc, char* argv[]) {
  // Traverse all arguments
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument.find("help") != std::string::npos) {
      std::cout << usage;
      return false;
    }
  }

  if (argc >= 2) {
    this->port = argv[1];
  }

  return true;
}

void HttpServer::stop() {
  // Stop listening for incoming client connection requests
  this->stopListening();
}
///---HANDLE REQUEST WILL BE THE JOB FOR THE HANDLERS---///
bool HttpServer::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  // Print IP and port from client
  const NetworkAddress& address = httpRequest.getNetworkAddress();
  Log::append(Log::INFO, "connection",
    std::string("connection established with client ") + address.getIP()
    + " port " + std::to_string(address.getPort()));

  // Print HTTP request
  // reports what client is asking for
  Log::append(Log::INFO, "request", httpRequest.getMethod()
    + ' ' + httpRequest.getURI()
    + ' ' + httpRequest.getHttpVersion());
  // send the request to the corresponding webapp
  return this->route(httpRequest, httpResponse);
}
///---ROUTE WILL BE THE JOB FOR THE HANDLERS TOO---///
bool HttpServer::route(HttpRequest& httpRequest, HttpResponse& httpResponse) {
  // Traverse the chain of applications
  for (size_t index = 0; index < this->applications.size(); ++index) {
    // If this application handles the request
    // i ask "is this the kind of request you can handle?"
    HttpApp* app = this->applications[index];
    if (app->handleHttpRequest(httpRequest, httpResponse)) {
      return true;
    }
  }

  // Unrecognized request
  return this->serveNotFound(httpRequest, httpResponse);
}
// to answer the request we need the request and the response to fill
bool HttpServer::serveNotFound(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setStatusCode(404); //not found, 200 is ok,300 is redirect
  // 400 is bad request, 500 is internal server error, 100 is information.
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Not found";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} h1 {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <p>The requested resouce was not found on this server.</p>\n"
    << "  <hr><p><a href=\"/\">Homepage</a></p>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}
