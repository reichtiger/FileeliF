
#include <wx/wx.h>
#include "licenseFrame.h"
#include "mainFrame.h"
#include "testFrame.h"

#include <stdio.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>

#pragma warning(disable:4996)

#define LOCAL_FILE      "test.txt"
#define UPLOAD_FILE_AS  "while-uploading.txt"
#define REMOTE_URL      "sftp://reichhome.ddns.info:33333/home/reichpi/"
#define RENAME_FILE_TO  "renamed-and-fine.txt"
mainFrame* gMainFrm = nullptr;
int testCURL();
class MyApp : public wxApp
{
    wxLocale m_locale;
	wxString m_dirToWatch;
public:
	// override base class virtuals
// ----------------------------

// this one is called on application startup and is a good place for the app
// initialization (doing it here and not in the ctor allows to have an error
// return: if OnInit() returns false, the application terminates)
	virtual bool OnInit() wxOVERRIDE;

	//void  OnEventLoopEnter(wxEventLoopBase* WXUNUSED(loop)) wxOVERRIDE;
	void  OnInitCmdLine(wxCmdLineParser& parser) wxOVERRIDE;
	bool  OnCmdLineParsed(wxCmdLineParser& parser) wxOVERRIDE;
	void  OnEventLoopEnter(wxEventLoopBase* WXUNUSED(loop)) wxOVERRIDE;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;

#ifdef WIN32
	AllocConsole();// need #pragma warning(disable:4996) in code
	freopen("CONOUT$", "w", stdout);
	std::cout << "started console " << std::endl;
#endif // WIN32
	//testCURL();return false;
	wxSizerFlags::DisableConsistencyChecks();
	if (m_locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_LOAD_DEFAULT))
	{
		// multi-language support
		//wxLocale::AddCatalogLookupPathPrefix("E:\\bin\\Debug\\lang");
		//wxLocale::AddCatalogLookupPathPrefix(".\\lang");//这样也行。上面的代码目录结构更清晰些。
		//在E:\\bin\\Debug\\lang中建立zh_CN文件夹
		//在E : \\bin\\Debug\\lang\\zh_CN文件夹里放置myApp.mo文件
		//注意：myApp.mo文件可以使用PoEdit工具生成

		m_locale.AddCatalog(wxT("FileeliF"));
	}

	wxInitAllImageHandlers();

	wxFile licFile;
	if (licFile.Exists("fflicense.txt")) {

		//testFrame* tf = new testFrame(wxT("testing"));tf->Show(true);

		gMainFrm = new mainFrame(wxT("FileeliF"));
		gMainFrm->loginFrm = nullptr;
		gMainFrm->account = wxT("test");
		gMainFrm->passwd = wxT("123");
		gMainFrm->Show(true);
	}
	else {
		licenseFrame* lic = new licenseFrame(wxT("FileeliF License"));

		lic->Show(true);
	}

	return true;
}

void  MyApp::OnEventLoopEnter(wxEventLoopBase* WXUNUSED(loop))
{
	
}

void  MyApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	wxApp::OnInitCmdLine(parser);
	parser.AddParam("directory to watch",
		wxCMD_LINE_VAL_STRING,
		wxCMD_LINE_PARAM_OPTIONAL);
}

bool  MyApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (!wxApp::OnCmdLineParsed(parser))
		return false;

	if (parser.GetParamCount())
		m_dirToWatch = parser.GetParam();

	return true;
}

void gloale_init()
{

	curl_global_init(CURL_GLOBAL_DEFAULT);
	return;
}

size_t read_callback(void* ptr, size_t size, size_t nmemb, void* stream) //回调函数
{
	curl_off_t nread;
	size_t retcode = fread(ptr, size, nmemb, (FILE*)(stream));
	nread = (curl_off_t)retcode;
	cout << nread << " bytes read" << endl;
	return retcode;
}

size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
	int written = fwrite(ptr, size, nmemb, (FILE*)stream);
	return written;
}

size_t upload(const char* user, const char* passwd, const char* url, const char* path)
{
	CURL* curl;
	CURLcode res;
	struct stat file_info;
	string s1(user);
	string s2(passwd);
	string s3 = s1 + ":" + s2;
	cout << s3 << endl;
	FILE* pSendFile = fopen(path, "r");
	if (pSendFile == NULL)
	{
		printf("open failed\n");
		return 1;
	}
	/* to get the file size */
	if (fstat(fileno(pSendFile), &file_info) != 0)
		return 1; /* cannot continue */

	curl = curl_easy_init();
	printf("curl_easy_init success\n");
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_USERPWD, s3.c_str());
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_READDATA, pSendFile);
		curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 0);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
			(curl_off_t)file_info.st_size);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		printf("curl_easy_setopt success\n");
		res = curl_easy_perform(curl);

		if (CURLE_OK != res)
		{
			fprintf(stdout, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		}

		fprintf(stdout, "curl_easy_perform() OK =============== : %s\n",
			curl_easy_strerror(res));
		curl_easy_cleanup(curl);

	}
	fclose(pSendFile);
	curl_global_cleanup();
	return 0;
}


//int download(const char* user, const char* passw, const char* url, const char* filePath)
//{
//	CURL* curl;
//	CURLcode curl_code;
//	string s1(user);
//	string s2(passw);
//	string s3 = s1 + ":" + s2;
//	cout << s3 << endl;
//	curl = curl_easy_init();
//	curl_easy_setopt(curl, CURLOPT_URL, url);
//	//    curl_easy_setopt(curl, CURLOPT_USERPWD, s3.c_str());
//	curl_easy_setopt(curl, CURLOPT_USERNAME, user);
//	curl_easy_setopt(curl, CURLOPT_PASSWORD, passw);
//
//	FILE* fp = fopen(filePath, "wb+");
//	if (NULL == fp)
//	{
//		curl_easy_cleanup(curl);
//		printf("fopen failed\n");
//		return -1;
//	}
//
//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
//	curl_code = curl_easy_perform(curl);
//	printf("curl_code = %d\n", curl_code);
//	if (CURLE_OK != curl_code)
//	{
//		printf("perform failed\n");
//		curl_easy_cleanup(curl);
//		fclose(fp);
//		remove(filePath);
//		return -1;
//	}
//	curl_easy_cleanup(curl);
//
//	fclose(fp);
//
//	return 0;
//}


int testCURL()
{
	gloale_init();
	//char* hostname = "reichhome.ddns.info";
	char* hostname = "192.168.0.102";
	char* port = "22";
	char* serverpath = "/home/reichpi/ttt.txt";
	char* user = "reichpi";
	char* passwd = "123qwe";
	char* uploadFile = "test.txt";
	char url[125] = { 0 };

	sprintf(url, "sftp://%s:%s/%s", hostname, port, serverpath);
	printf("url: %s\n", url);
	//download(user,passwd,url,savepath);
	upload(user, passwd, url, uploadFile);
	return 0;
}