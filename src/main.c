/* author: snikons */

/* printf(), vprintf() */
#include <stdio.h>

/* va_start(), va_end() */
#include <stdarg.h>

/* glib */
#include <glib.h>
#include <gmodule.h>

/* socket(), recvfrom(), AF_INET, SOCK_RAW */
#include <sys/socket.h>

/* IPPROTO_TCP */
#include <netinet/in.h>

/* struct iphdr */
#include <netinet/ip.h>

/* close() */
#include <unistd.h>

/* errno */
#include <errno.h>

/* strerror() */
#include <string.h>

/* malloc() */
#include <stdlib.h>

/* assert() */
#include <assert.h>

#define INDENT "    "

void log_msg(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  vprintf(format, ap);
  printf("\n");
  va_end(ap);
}

int log_ip_header(void *msg, size_t msg_len) {
  struct iphdr *header = (struct iphdr *)msg;

  assert(msg != NULL && msg_len > 0);
  log_msg(INDENT "IP header {");
  log_msg(INDENT INDENT "IP version = %u", (unsigned int)(header->version));
  /* ihl represents length in 32 bit words */
  log_msg(INDENT INDENT "header length = %u bytes", (unsigned int)(header->ihl * 4));
  log_msg(INDENT "}");

  return 0;
}

int log_tcp_packet(void *msg, size_t msg_len) {
  log_ip_header(msg, msg_len);
  return 0;
}

int log_packet(void *msg, size_t msg_len) {
  static size_t id = 0;
  struct iphdr *header = (struct iphdr *)msg;

  assert(msg != NULL && msg_len > 0);
  ++id; /* todo: detect and handle overflow */

  /* ref: https://tools.ietf.org/html/rfc791 page 14 */
  /* ref: https://tools.ietf.org/html/rfc790 page 5 */
  switch(header->protocol) {
    case 6: /* TCP */
      log_msg("%zu: TCP msg of length %zu bytes", id, msg_len);
      return log_tcp_packet(msg, msg_len);

    default:
      log_msg("%zu: Unknown msg of length %zu bytes", id, msg_len);
      return 0;
  }
}

int main(int argc, char* argv[]) {
  int sock = -1;
  ssize_t msg_len = 0;
  void *msg = NULL;
  const size_t MSG_BUFFER_LEN = 65536; /* todo: verify */

  /* allocate memory for a msg */
  msg = malloc(MSG_BUFFER_LEN);
  if (!msg)
    g_error("malloc(%zu) failed with error %d: %s", MSG_BUFFER_LEN, errno,
      strerror(errno));

  /* create a socket */
  sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
  if (sock < 0)
    g_error("socket(AF_INET, SOCK_RAW, IPPROTO_TCP) failed with error %d: %s",
      errno, strerror(errno));

  /* receive packets */
  while(1) {
    msg_len = recvfrom(sock, msg, MSG_BUFFER_LEN, 0, NULL, 0);
    if (msg_len < 0)
      g_error("recvfrom(sock, msg, MSG_BUFFER_LEN, 0, NULL, 0) "
        "failed with error %d: %s", errno, strerror(errno));

    if (msg_len == 0) break; /* no messages available */
    if (log_packet(msg, msg_len))
      g_error("log_packet() failed with error %d: %s", errno, strerror(errno));
  }

  free(msg);
  close(sock);
  return 0;
}