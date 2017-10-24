// parses event using stream parser

// builds a tree from selected nodes
// builds an index to banks in the event, searches index for particular banks

// in principle more than one bank may have the same tagNum,
//   in this case must use the getRange() method to get them all, see Doxygen doc


// ejw, 1-may-2012


#include <iostream>
#include <stdio.h>
#include "evioUtil.hxx"
#include "evioFileChannel.hxx"
#include "evioBankIndex.hxx"


using namespace evio;
using namespace std;

class InputParser
{
public:
  InputParser(int &argc, char **argv)
  {
    for (int i = 1; i < argc; ++i)
      this->tokens.push_back(string(argv[i]));
  }
  /// @author iain
  const string getCmdOption(const string & option) const
  {
    vector < string >::const_iterator itr;
    itr = find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end())
      {
	return *itr;
      }
    return "";
  }
  /// @author iain
  bool cmdOptionExists(const string & option) const
  {
    return find(this->tokens.begin(), this->tokens.end(), option)
      != this->tokens.end();
  }
private:
    vector < string > tokens;
};

//--------------------------------------------------------------
//--------------------------------------------------------------


int
main(int argc, char **argv)
{
  InputParser input(argc, argv);
  int32_t usertag = 0, usernum = 0;

  if(input.cmdOptionExists("-t"))
    {
      usertag = strtol(input.getCmdOption("-t").c_str(), NULL, 10);
    }

  if(input.cmdOptionExists("-n"))
    {
      usernum = strtol(input.getCmdOption("-n").c_str(), NULL, 10);
    }

  try
  {

    // create and open file channel
    evioFileChannel *chan;
    if (argc > 1)
      {
	chan = new evioFileChannel(argv[argc-1], "r");
      }
    else
      {
	chan = new evioFileChannel("fakeEvents.dat", "r");
      }
    chan->open();

    int32_t version = 0;
    int stat = chan->ioctl("V",&version);
    if(stat == 0)
      cout << "# EVIO Version  " << version << endl;

    uint32_t *buffer, blen;
    evioDictEntry tn;

    while (chan->readAlloc((uint32_t **)&buffer, &blen))
      {
	uint64_t eventNumber=0;
	uint64_t evType=0;
	const uint64_t *d64;
	const uint32_t *d32;
	evioBankIndex bi(buffer);

	int len;

	if(version < 4)
	  {
	    tn = evioDictEntry(0xC000, 0);
	    d32 = bi.getData < uint32_t >(tn, &len);
	    if(d32 != NULL)
	      {
		eventNumber = (uint32_t)d32[0];
		evType   = (uint32_t)d32[1];
	      }
	  }
	else
	  {
	    tn = evioDictEntry(0x0, 0);
	    d64 = bi.getData < uint64_t >(tn, &len);
	    if(d64 != NULL)
	      {
		eventNumber = d64[0];
		evType   = d64[1];
	      }
	  }

	// Check to see if this event falls into the user specified range
	if(eventNumber)
	  {
	    printf("# %6d: evType = %d\n",
		   (uint32_t)eventNumber, (uint32_t)evType);
	  }

	// Get the ROC / Payload data
	tn = evioDictEntry(usertag, usernum);

	d32 = bi.getData < uint32_t >(tn, &len);

	if (d32 != NULL)
	  {
	    for (int i = 0; i < len; i++)
	      printf("0x%08x  ", d32[i]);
	    cout << endl;
	  }
	// else
	//   {
	//     cout << "?cannot find <uint32_t> data for: " << tn.first << "," <<
	//       (int) tn.second << endl;
	//   }

	free(buffer);
      }


  }
  catch(evioException e)
  {
    cerr << e.toString() << endl;
    exit(EXIT_FAILURE);
  }

  exit(0);
  printf("%s", DataTypeNames[0]);
}


//--------------------------------------------------------------
//--------------------------------------------------------------
