const String PAGE_HEADER =
        "<!DOCTYPE html>"
          "<html lang=\"en\">"
          "<head>"
              "<meta charset=\"UTF-8\">"
              "<title>Sonoff Configuration</title>"
              "<style>"
                  "body{font-family:tahoma;}"
              "</style>"
          "</head>"
          "<body>"
          "<h2>Sonoff switch</h2>";

const String PAGE_FOOTER =
          "</body>"
          "</html>";
        
          
void handleRoot() {
  Serial << "Server: root requested" << endl;
  String page = 
          "<h4>Options:</h4>"
          "<ul>"
              "<li><a href=\"/configure\">Change configuration</a></li>"
              "<li><a href=\"/update\">Update firmware</a></li>"
          "</ul>";

  generatePage(page);
}

void handleConfiguration() {
  Serial << "Server: configuration" << endl;
  String page = "Configuration";
  generatePage(page);
}

void handleUpdate() {
  Serial << "Server: configuration" << endl;
  String page = "Firmware update";
  generatePage(page);
}




void handleNotFound(){
  Serial << "Server: page not found" << endl;
  String page = "Page not found";
  generatePage(page);
}


void generatePage(String &page) {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  page = PAGE_HEADER + page + PAGE_FOOTER;
  server.send(200, "text/html", page);
}
