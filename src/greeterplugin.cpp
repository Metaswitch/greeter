/**
 * @file greeterplugin.cpp
 *
 * Project Clearwater - IMS in the Cloud
 * Copyright (C) 2015  Metaswitch Networks Ltd
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version, along with the "Special Exception" for use of
 * the program along with SSL, set forth below. This program is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * The author can be reached by email at clearwater@metaswitch.com or by
 * post at Metaswitch Networks Ltd, 100 Church St, Enfield EN2 6BQ, UK
 *
 * Special Exception
 * Metaswitch Networks Ltd  grants you permission to copy, modify,
 * propagate, and distribute a work formed by combining OpenSSL with The
 * Software, or a work derivative of such a combination, even if such
 * copying, modification, propagation, or distribution would otherwise
 * violate the terms of the GPL. You must comply with the GPL in all
 * respects for all of the code used other than OpenSSL.
 * "OpenSSL" means OpenSSL toolkit software distributed by the OpenSSL
 * Project and licensed under the OpenSSL Licenses, or a work based on such
 * software and licensed under the OpenSSL Licenses.
 * "OpenSSL Licenses" means the OpenSSL License and Original SSLeay License
 * under which the OpenSSL Project distributes the OpenSSL toolkit software,
 * as those licenses appear in the file LICENSE-OPENSSL.
 */

#include "cfgoptions.h"
#include "sproutletplugin.h"
#include "sproutletappserver.h"
#include "greeterappserver.h"

class GreeterPlugin : public SproutletPlugin
{
public:
  GreeterPlugin();
  ~GreeterPlugin();

  bool load(struct options& opt, std::list<Sproutlet*>& sproutlets);
  void unload();

private:
  GreeterAppServer* _greeter;
  SproutletAppServerShim* _greeter_sproutlet;
};

GreeterPlugin::GreeterPlugin() :
  _greeter(NULL),
  _greeter_sproutlet(NULL)
{
}

GreeterPlugin::~GreeterPlugin()
{
}

/// Export the plug-in using the magic symbol "sproutlet_plugin"
extern "C" {
GreeterPlugin sproutlet_plugin;
}

/// Loads the Greeter plug-in, returning the supported Sproutlets.
bool GreeterPlugin::load(struct options& opt, std::list<Sproutlet*>& sproutlets)
{
  bool plugin_loaded = true;

  TRC_STATUS("Loading Greeter!");

  // Create the App Server. This uses hard-coded configuration
  std::string greeter_uri = "sip:greeter." + opt.sprout_hostname + ":5060;transport=tcp";
  _greeter = new GreeterAppServer("greeter");
  _greeter_sproutlet = new SproutletAppServerShim(_greeter, 5060, greeter_uri);

  sproutlets.push_back(_greeter_sproutlet);

  return plugin_loaded;
}

/// Unloads the Greeter plug-in.
void GreeterPlugin::unload()
{
  delete _greeter_sproutlet;
  delete _greeter;
}
