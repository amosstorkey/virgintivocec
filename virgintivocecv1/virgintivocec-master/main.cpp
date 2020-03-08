/*
 * VirginTivoCec
 *
 * Copyright Amos Storkey 2015 a.j.storkey@ed.ac.uk 
 *
 * Based on cecanway Magnus Kulke, used under GNU GP Licence 2 or later.
 *
 * This program is released and can be redistributed and/or modified
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include "libcec/cec.h"
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

using namespace CEC;
using namespace std;

#define CEC_CONFIG_VERSION LIBCEC_VERSION_CURRENT;
#include "libcec/cecloader.h"

ICECCallbacks        callbacks;
libcec_configuration configuration;
string               port;
bool                 aborted;
bool                 daemonize;
bool                 logEvents;
string               configFilePath;
const char*          virginIp;
const char*          virginName;
unsigned int         virginPort;
map<int, string>     virginMap;
int PressKey(const string json);

void populateKeyMapDefault()
{
  cout << "Building Map" <<endl;
  virginMap[CEC_USER_CONTROL_CODE_PAGE_UP] = "IRCODE CHANNELUP\r\n";
  virginMap[CEC_USER_CONTROL_CODE_PAGE_DOWN] = "IRCODE CHANNELDOWN\r\n";
  virginMap[CEC_USER_CONTROL_CODE_CHANNEL_UP] = "IRCODE CHANNELUP\r\n";
  virginMap[CEC_USER_CONTROL_CODE_CHANNEL_DOWN] = "IRCODE CHANNELDOWN\r\n";
  virginMap[CEC_USER_CONTROL_CODE_LEFT] = "IRCODE LEFT\r\n";
  virginMap[CEC_USER_CONTROL_CODE_RIGHT] = "IRCODE RIGHT\r\n";
  virginMap[CEC_USER_CONTROL_CODE_DOWN] = "IRCODE DOWN\r\n";
  virginMap[CEC_USER_CONTROL_CODE_UP] = "IRCODE UP\r\n";
  virginMap[CEC_USER_CONTROL_CODE_SELECT] = "IRCODE SELECT\r\n";
  virginMap[CEC_USER_CONTROL_CODE_EXIT] = "IRCODE LIVETV\r\n";
  virginMap[CEC_USER_CONTROL_CODE_NUMBER0] = "IRCODE NUM0\r\n";
  virginMap[CEC_USER_CONTROL_CODE_NUMBER1] = "IRCODE NUM1\r\n";
  virginMap[CEC_USER_CONTROL_CODE_NUMBER2] = "IRCODE NUM2\r\n";
  virginMap[CEC_USER_CONTROL_CODE_NUMBER3] = "IRCODE NUM3\r\n";
  virginMap[CEC_USER_CONTROL_CODE_NUMBER4] = "IRCODE NUM4\r\n";
  virginMap[CEC_USER_CONTROL_CODE_NUMBER5] = "IRCODE NUM5\r\n";
  virginMap[CEC_USER_CONTROL_CODE_NUMBER6] = "IRCODE NUM6\r\n";
  virginMap[CEC_USER_CONTROL_CODE_NUMBER7] = "IRCODE NUM7\r\n";
  virginMap[CEC_USER_CONTROL_CODE_NUMBER8] = "IRCODE NUM8\r\n";
  virginMap[CEC_USER_CONTROL_CODE_NUMBER9] = "IRCODE NUM9\r\n";
  virginMap[CEC_USER_CONTROL_CODE_RECORD] = "IRCODE RECORD\r\n";
  virginMap[CEC_USER_CONTROL_CODE_PLAY] = "IRCODE PLAY\r\n";
  virginMap[CEC_USER_CONTROL_CODE_FAST_FORWARD] = "IRCODE ADVANCE\r\n";
  virginMap[CEC_USER_CONTROL_CODE_REWIND] = "IRCODE REPLAY\r\n";
  virginMap[CEC_USER_CONTROL_CODE_STOP] = "IRCODE STOP\r\n";
  virginMap[CEC_USER_CONTROL_CODE_PAUSE] = "IRCODE PAUSE\r\n";
  virginMap[CEC_USER_CONTROL_CODE_ELECTRONIC_PROGRAM_GUIDE] = "IRCODE GUIDE\r\n";
  virginMap[CEC_USER_CONTROL_CODE_F1_BLUE] = "IRCODE ACTION_B\r\n";
  virginMap[CEC_USER_CONTROL_CODE_F2_RED] = "IRCODE ACTION_C\r\n";
  virginMap[CEC_USER_CONTROL_CODE_F3_GREEN] = "IRCODE ACTION_D\r\n";
  virginMap[CEC_USER_CONTROL_CODE_F4_YELLOW] = "IRCODE ACTION_A\r\n";
  virginMap[CEC_USER_CONTROL_CODE_AN_RETURN] = "IRCODE DISPLAY\r\n";
  virginMap[CEC_USER_CONTROL_CODE_PREVIOUS_CHANNEL] = "IRCODE ENTER\r\n";
  virginMap[CEC_USER_CONTROL_CODE_AN_CHANNELS_LIST] = "IRCODE TIVO\r\n";
}

int CecCommandCB(void*, const cec_command command)
{
  cout << "CeC Command" << endl;
  cout << "commandissued " << command.opcode << "versus" << CEC_OPCODE_DECK_CONTROL << " option " << int(command.parameters[0]) << int(CEC_USER_CONTROL_CODE_AN_RETURN) <<endl;
  switch (command.opcode)
  {
    case CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN:
      if (command.parameters.size ==1 &&
           command.initiator == CECDEVICE_TV)
        if (command.parameters[0] == CEC_USER_CONTROL_CODE_AN_RETURN)
        {
          string json = virginMap[CEC_USER_CONTROL_CODE_AN_RETURN];
          PressKey(json);
        }
        if (command.parameters[0] == CEC_USER_CONTROL_CODE_AN_CHANNELS_LIST)
        {
          string json = virginMap[CEC_USER_CONTROL_CODE_AN_CHANNELS_LIST];
          PressKey(json);
        }
      cout << "button down" << endl;
      break;
    case CEC_OPCODE_USER_CONTROL_PRESSED:
    if (command.initiator == CECDEVICE_TV &&
      command.parameters.size ==1 &&
      command.parameters[0] == CEC_USER_CONTROL_CODE_STOP)
    {
    string json = virginMap[CEC_USER_CONTROL_CODE_STOP];
    PressKey(json);
    }
/*    case CEC_OPCODE_DECK_CONTROL:
    if (command.initiator == CECDEVICE_TV &&
      command.parameters.size ==1 &&
      command.parameters[0] == CEC_DECK_CONTROL_MODE_STOP)
    {
    string json = virginMap[CEC_USER_CONTROL_CODE_STOP];
    PressKey(json);
    }
    if (command.initiator == CECDEVICE_TV &&
       command.parameters.size ==1 &&
       command.parameters[0] == CEC_DECK_CONTROL_MODE_SKIP_FORWARD_WIND)
    {
      string json = virginMap[CEC_USER_CONTROL_CODE_PLAY];
      PressKey(json);
    }
    if (command.initiator == CECDEVICE_TV &&
      command.parameters.size ==1 &&
      command.parameters[0] == CEC_DECK_CONTROL_MODE_SKIP_REVERSE_REWIND)
    {
    string json = virginMap[CEC_USER_CONTROL_CODE_PAUSE];
    PressKey(json);
    }
    break;
*/
    default:
    cout << "CEC Command End: Nothing Special" << endl;
    break;
  }
}

int CecKeyPressCB(void*, const cec_keypress key)
{
  cout << "CeCKeyPress" << endl;
  if (key.duration == 0)
  {
    cout << "key " <<  key.keycode << endl;
    string json = "unmapped";
    if (virginMap.find(key.keycode) != virginMap.end())
    {
      json = virginMap[key.keycode];
      PressKey(json);
    }
   
    if (logEvents)
      cout << "keycode: " << key.keycode << ", virgin command: " << json << endl;
  }
    
  return 0;
}

int PressKey(const string json)
{
      cout << "Pressing key" <<endl;
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0)
      {
        cout << "error opening socket" << endl;
        return 1;
      }
      
      struct sockaddr_in serv_addr; 
      memset(&serv_addr, '0', sizeof(serv_addr)); 
      serv_addr.sin_family = AF_INET;
      inet_pton(AF_INET, virginIp, &serv_addr.sin_addr);
      serv_addr.sin_port = htons(virginPort);
 
      if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr_in)) < 0)
      {
        cout << "error connecting to Virgin box" << virginIp <<" port " << virginPort << endl;
        return 1;
      }

      if ( write(sockfd, json.c_str(), json.length())<0 )
      {
      cout << "error writing to socket " << errno << endl; 
      return 1;
      }
      cout << "sent to virgin box" << endl;
      
      shutdown(sockfd, SHUT_WR); 
 
      close(sockfd);
      return 0;
}

void sighandler(int iSignal)
{
  cout << "signal caught: " <<  iSignal << " - exiting" << endl;
  aborted = true;
}

void parseOptions(int argc, char* argv[])
{  
  cout << "parse options" << endl;
  stringstream ss;
  ss << argv[0];
  ss << " [-d] (daemonize) [-l] (log keypresses) [-h] (help)";
  string usage = ss.str();
  
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-d") == 0) 
      daemonize = true;
    else if (strcmp(argv[i], "-l") == 0) 
      logEvents = true;
    else
    {
      cout << usage << endl;
      exit((strcmp(argv[i], "-h") == 0) ? 0 : 1);
    }
  }
}


int main (int argc, char* argv[])
{
  daemonize = false;
  logEvents = false;
  configFilePath = "/etc/virgintivocec.conf";
  virginPort = 31339;
  virginIp = "192.168.0.16";  
  virginName = "Virgin TIVO";
  parseOptions(argc, argv);  
 
  populateKeyMapDefault();
  
  
 
  if (daemonize)
  {
    pid_t pid;
    if ((pid = fork()) < 0) 
    {
      cout << "cannot fork" << endl;
      return 1;
    } 
    else if (pid != 0) 
    { 
      // parent
      exit(0);
    }
    setsid();
  }
  
  if (signal(SIGINT, sighandler) == SIG_ERR)
  {
    cout << "can't register sighandler" << endl;
    return -1;
  }

  configuration.Clear();
  callbacks.Clear();
  snprintf(configuration.strDeviceName, 13, virginName);
  configuration.clientVersion = CEC_CONFIG_VERSION;
  configuration.baseDevice = CECDEVICE_AUDIOSYSTEM;
  configuration.iHDMIPort  = 1;
  configuration.bActivateSource = 1;
  configuration.iPhysicalAddress = 1100;
  callbacks.CBCecKeyPress = &CecKeyPressCB;
  callbacks.CBCecCommand = &CecCommandCB;
  configuration.callbacks = &callbacks;
  
  configuration.deviceTypes.Add(CEC_DEVICE_TYPE_RECORDING_DEVICE);

  ICECAdapter *parser = LibCecInitialise(&configuration);
  if (!parser)
  {
    cout << "Cannot load libcec.so" << endl;
    return 1;
  }

  // init video on targets that need this
  parser->InitVideoStandalone();

  cout << "autodetect serial port: ";
  cec_adapter devices[10];
  uint8_t iDevicesFound = parser->FindAdapters(devices, 10, NULL);
  if (iDevicesFound <= 0)
  {
    cout << "FAILED" << endl;
    UnloadLibCec(parser);
    return 1;
  }
  else
  {
    port = devices[0].comm;
    cout << port << endl;
  }

  cout << "opening a connection to the CEC adapter..." << endl;

  if (!parser->Open(port.c_str()))
  {
    cout << "unable to open the device on port " << port << endl;
    UnloadLibCec(parser);
    return 1;
  }
  cout << "opened CEC adapter" << endl;
  pause();

  parser->Close();

  UnloadLibCec(parser);

  return 0;
}
