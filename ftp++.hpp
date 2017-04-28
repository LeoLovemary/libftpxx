/*******************************************************************************
 *  FTP access library (ftplib C++ wrapper)
 *  
 *  © 2017, Sauron
 ******************************************************************************/

#ifndef __FTPXX_HPP
#define __FTPXX_HPP

#include <ftplib.h>
#include <string>
#include <sys/types.h>

/** Exception during establishing connection to FTP server **/
class FTPConnectException {};

/** Exception during remote procedure call **/
class FTPException {
public:
    /** Initialize with last server response **/
    FTPException(const char * lastResponse) : lastResponse(lastResponse) {}
    /** Returns last server response **/
    operator const char *() const { return lastResponse; }
    
private:
    const char * lastResponse;
};

/** Base class for FTP connections **/
class FTPConnectionBase {
public:
    /** Data transfer mode **/
    enum TransferMode {
        TEXT=FTPLIB_TEXT,
        BINARY=FTPLIB_BINARY
    };
    /** Close FTP connection **/
    virtual ~FTPConnectionBase();
    
protected:
    netbuf * conn;
};

/** FTP main (control) connection **/
class FTPConnection : public FTPConnectionBase {
    friend class FTPDataConnection;
public:
    /** Connection mode **/
    enum Mode {
        PASSIVE=FTPLIB_PASSIVE,
        PORT=FTPLIB_PORT
    };
    /** Create new FTP connection **/
    FTPConnection(const char * host);
    /** Returns last response **/
    const char * getLastResponse();
    /** Set connection mode **/
    void setConnectionMode(Mode mode);
    /** Log in to the server **/
    void login(const char * user, const char * password);
    /** Returns system type **/
    std::string getSystemType();
    /** Returns current directory **/
    std::string getDirectory();
    /** Change current directory **/
    void cd(const char * directory);
    /** Move up one level **/
    void cdup();
    /** Get terse directory listing **/
    void getList(const char * filename, const char * path);
    /** Get detailed directory listing **/
    void getFullList(const char * filename, const char * path);
    /** Make a directory **/
    void mkdir(const char * directory);
    /** Remove a directory **/
    void rmdir(const char * directory);
    /** Rename or move file **/
    void rename(const char * oldName, const char * newName);
    /** Delete a file **/
    void unlink(const char * filename);
    /** Download a file **/
    void get(const char * local, const char * remote, TransferMode mode);
    /** Upload a file **/
    void put(const char * local, const char * remote, TransferMode mode);
    /** Get file size **/
    unsigned size(const char * path, TransferMode mode);
    /** Get file modification date/time **/
    std::string modDate(const char * path);
    /** Run site-dependent command **/
    void site(const char * command);
};

/** FTP data connection **/
class FTPDataConnection : public FTPConnectionBase {
public:
    /** Data connection type **/
    enum Type {
        DIR=FTPLIB_DIR,
        DIR_VERBOSE=FTPLIB_DIR_VERBOSE,
        FILE_READ=FTPLIB_FILE_READ,
        FILE_WRITE=FTPLIB_FILE_WRITE
    };
    /** Open FTP data connection **/
    FTPDataConnection(FTPConnection &connection, const char * path, Type type,
        TransferMode mode);
    /** Read data from a remote file or directory **/
    ssize_t read(void * buffer, size_t length);
    /** Write data to a remote file **/
    ssize_t write(void * buffer, size_t length);
};

#endif