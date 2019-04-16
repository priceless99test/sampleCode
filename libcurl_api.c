#include <netinet/in.h>
#include <arpa/inet.h>
#include <curl/curl.h>



#if 1
/* /usr/bin/curl -v --stderr "/tmp/discover_slp.log" -X POST -H "Content-Type: application/json" -d @/tmp/discover_slp.json -k --tlsv1.2 https://1.1.1.1/registerRequest */
bool PostToLXCATargetAdvanced( char* target )
{
    CURL *curl;
    CURLcode curlCode;
    struct curl_slist* hs = NULL;
    struct data config;
    
    config.trace_ascii = 1; /* enable ascii tracing */


    char lxcaURL[128] = {0};
    bool ret = false;

    fprintf( stderr, "==== PostToLXCATargetAdvanced start ====\n" );

    if( NULL == target )
    {
        PRINT_LOG( PRINT_ERR, "NULL pointer.\n" );
        return ret;
    }

    snprintf( lxcaURL, sizeof(lxcaURL), "https://%s/registerRequest", target );

    hs = curl_slist_append( hs, "Content-Type: application/json" );
    curl = curl_easy_init();
    if( curl ) 
    {
#if 0
        FILE *fp = fopen( "/tmp/discover_slp_curllib.log", "wb" );

        if( NULL == fp )
        {
            return ret;
        }
        if( (curlCode=curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L)) == CURLE_OK && /* ask libcurl to show us the verbose output */
            (curlCode=curl_easy_setopt(curl, CURLOPT_URL, lxcaURL)) == CURLE_OK && /* lxca url */
            (curlCode=curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2)) == CURLE_OK && /* ask libcurl to use TLS version 1.2 or later */
            (curlCode=curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "@/tmp/discover_slp.json")) == CURLE_OK && /* now specify the POST data */
            (curlCode=curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs)) == CURLE_OK && /* specify the libcurl */
            (curlCode=curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L)) == CURLE_OK && /* curl doesn't verify the authenticity of the peer's certificate with value 0L */
            //(curlCode=curl_easy_setopt(curl, CURLOPT_STDERR, fp)) == CURLE_OK &&
            //(curlCode=curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L)) == CURLE_OK &&
            (curlCode=curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L)) == CURLE_OK ) /* the connection succeeds regardless of the names in the certificate. Use that ability with caution! */
        {
            PRINT_LOG( PRINT_INFO, "curl_easy_setopt is successful.\n" );
            ret = true;
        }
        else
        {
            PRINT_LOG( PRINT_ERR, "curl_easy_setopt() failed: %s\n", curl_easy_strerror(curlCode) );
        }
#endif
#if 1
    FILE *dfp = fopen( "/tmp/test.json", "r" );
    struct stat file_info;
    if( NULL == dfp )
    {
        return ret;
    }
    if(fstat(fileno(dfp), &file_info) != 0) {
        fprintf(stderr, "Could not get file information.\n");
        return ret;
    }
    if( (curlCode=curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L)) == CURLE_OK && /* ask libcurl to show us the verbose output */
        (curlCode=curl_easy_setopt(curl, CURLOPT_URL, "https://10.162.246.45/redfish/v1/Systems/1/LogServices/StandardLog/Actions/Oem/LenovoLogService.ClearSpecifiedLog")) == CURLE_OK && /* lxca url */
        (curlCode=curl_easy_setopt(curl, CURLOPT_READDATA, dfp)) == CURLE_OK && /* POST data is from a file */
        (curlCode=curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, file_info.st_size)) == CURLE_OK && /* POST data is from a file */
        (curlCode=curl_easy_setopt(curl, CURLOPT_POST, 1L)) == CURLE_OK && /* POST data is from a file */
        (curlCode=curl_easy_setopt(curl, CURLOPT_USERPWD, "USERID:PASSW0RD")) == CURLE_OK &&
        (curlCode=curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L)) == CURLE_OK && /* A long parameter set to 1 tells the library to fail the request if the HTTP code returned is equal to or larger than 400. */
        (curlCode=curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs)) == CURLE_OK && /* Content-Type change */
        (curlCode=curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2)) == CURLE_OK && /* ask libcurl to use TLS version 1.2 or later */
        (curlCode=curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L)) == CURLE_OK && /* curl doesn't verify the authenticity of the peer's certificate with value 0L */
        (curlCode=curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace)) == CURLE_OK && /* for debug */
        (curlCode=curl_easy_setopt(curl, CURLOPT_DEBUGDATA, &config)) == CURLE_OK && /* for debug */
        (curlCode=curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L)) == CURLE_OK ) /* the connection succeeds regardless of the names in the certificate. Use that ability with caution! */
    {
        PRINT_LOG( PRINT_INFO, "curl_easy_setopt is successful.\n" );
        ret = true;
    }
    else
    {
        PRINT_LOG( PRINT_ERR, "curl_easy_setopt() failed: %s\n", curl_easy_strerror(curlCode) );
    }
#endif

    }
    long http_code = 0;
#if 0
    if( ret &&
        (curlCode=curl_easy_perform(curl)) == CURLE_OK && /* Perform the request */
        (curlCode=curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code)) == CURLE_OK )
#endif
    if( ret && (curlCode=curl_easy_perform(curl)) != CURLE_OK ) 
    {
        // error
        fprintf( stderr, "==== a HTTP response error problem ====\n" );
    }
    else
    {
        // success
        fprintf( stderr, "==== a HTTP response success ====\n" );
    }

    if( (curlCode=curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code)) == CURLE_OK )
    {
        // http return code
        fprintf( stderr, "==== a HTTP response code (%ld) ====\n", http_code );
    }

    curl_easy_cleanup(curl);

    fprintf( stderr, "==== PostToLXCATargetAdvanced end ====\n" );

    return ret;
}
#endif