#include <fcgi_stdio.h>
#include <stdlib.h>

int main() {
  while (FCGI_Accept() >= 0) {
    printf("Content-Type: text/html\r\n\r\n");
    printf("<html><head><title>FastCGI Hello!</title></head><body>");
    printf("<h1>FastCGI Hello!</h1>");
    printf("<p>Request number %d running on host <i>%s</i></p>", FCGI_RequestId(), getenv("SERVER_NAME"));
    printf("</body></html>");
  }
  return 0;
}
