
/* This software was developed at the National Institute of Standards and
 * Technology by employees of the Federal Government in the course of their
 * official duties. Pursuant to title 17 Section 105 of the United States
 * Code this software is not subject to copyright protection and is in the
 * public domain. NIST assumes no responsibility whatsoever for its use by
 * other parties, and makes no guarantees, expressed or implied, about its
 * quality, reliability, or any other characteristic.

 * We would appreciate acknowledgement if the software is used.
 * The SAMATE project website is: http://samate.nist.gov
*/


#include <iostream>
#include <exception>
#include <string>
#include <map>
// Using Cgicc: http://cgicc.org (FastCGI compatible)
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

using namespace std;
using namespace cgicc;

int main()
{
	try
	{
		Cgicc cgi;
		cout << HTTPHTMLHeader() << endl;
		cout << HTMLDoctype(HTMLDoctype::eStrict) << endl;
		cout << html().set("lang", "EN").set("dir", "LTR") << endl;
		// Set up the HTML document
		cout << html() << head() << title("Test Case: XSS") << head() << endl;
		cout << body().set("bgcolor","#cccccc") << endl;
		cout << h1("Test Case: XSS") << endl;
		std::map<string, string> parameters;
		for(const_form_iterator iter = cgi.getElements().begin(); iter != cgi.getElements().end(); ++iter) {
			parameters[iter->getName()] = iter->getValue();
		}
		// list all values
		for (std::map<string,string>::const_iterator iter=parameters.begin();iter!=parameters.end();++iter)
			cout << iter->second << "<br />" << endl;
		cout << body() << html();
	}
	catch(const exception& e)
	{
		cout << "Exception: " << e.what() << endl;
	}
	return 0;	
}
