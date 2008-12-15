#
# SConstruct
# disko build script
#
# Process this file with 'scons' to build the project.
# For more information, please visit: http://www.scons.org/ .
#
# Usage:
#
#   . scons               build the library
#   . scons -h            see available configuration options
#   . scons opt=value     set a configuration option
#   . scons check         perform dependency checking
#   . scons install       install library and include files
#   . scons doc           build documentation for the project (Doxygen)
#

import os, sys

#######################################################################
# Scons configuration                                                 #
#######################################################################
SetOption('implicit_cache', 1)

#######################################################################
# Version                                                             #
#######################################################################
packageVersionMajor = 1
packageVersionMinor = 5
packageVersionMicro = 0
packageVersionRC    = '_rc2'

# Package information
packageName        = 'disko'
packageRealName    = 'Disko Framework'
packageDescription = 'Disko application framework for embedded devices (http://www.diskohq.org)'
packageVersion     = '%d.%d.%d%s' % (packageVersionMajor, packageVersionMinor, packageVersionMicro, packageVersionRC)

#######################################################################
# Help text                                                           #
#######################################################################
Help("Type: 'scons [options]' to build disko.\n" +
     "      'scons [options] check' to check the requirements for building disko.\n" +
     "      'scons install' to install disko.\n\n" +
     "The following options are available:\n")

#######################################################################
# Command line options                                                #
#######################################################################
opts = Options('disko.conf')
opts.AddOptions(
    PathOption('prefix',       'Installation directory', '/usr', PathOption.PathIsDirCreate),
    BoolOption('debug',        'Build with debug symbols and without optimize', False),
    BoolOption('messages',     'Build with logfile support', False),
    BoolOption('profile',      'Build with profiling support (includes debug option)', False),
    BoolOption('use_sse',      'Use SSE optimization', False),
    ListOption('graphics',     'Set graphics backend', 'none', ['dfb', 'x11']),
    ListOption('database',     'Set database backend', 'sqlite3', ['sqlite3', 'mysql', 'odbc']),
    BoolOption('enable_media', 'Build with mmsmedia support', True),
    BoolOption('enable_flash', 'Build with mmsflash support', False),
    BoolOption('enable_sip',   'Build with mmssip support', False),
    BoolOption('enable_mail',  'Build with email support', False),
    BoolOption('enable_tools', 'Build disko tools', False))

env = Environment(ENV = os.environ, CPPPATH = '../../../inc') 
opts.Update(env)
opts.Save('disko.conf', env)
Help(opts.GenerateHelpText(env))
env['TOP_DIR'] = os.getcwd()

# Here are our installation paths:
idir_prefix = '$prefix'
idir_lib    = '$prefix/lib/disko'
idir_bin    = '$prefix/bin'
idir_inc    = '$prefix/include/disko'
idir_data   = '$prefix/share/disko'

# extra flags
if env['messages']:
	env.Append(CCFLAGS = '-D__ENABLE_LOG__')

if env['profile']:
	env.Append(CCFLAGS = '-pg')
	env.Replace(debug = 1)

if env['debug']:
	env.Append(CCFLAGS = '-O0 -g -Wall -D__ENABLE_DEBUG__')
else:
	env.Append(CCFLAGS = '-O3')

if env['use_sse']:
	env.Append(CCFLAGS = '-msse2 -mfpmath=sse -D__HAVE_SSE__')

# format output
env['SHCXXCOMSTR']  = '  [CXX] $SOURCE'
env['SHLINKCOMSTR'] = '  [LD]  $TARGET'
env['CXXCOMSTR']    = '  [CXX] $SOURCE'
env['LINKCOMSTR']   = '  [LD]  $TARGET'

Export('env idir_prefix idir_lib idir_bin idir_inc idir_data')

#######################################################################
# Subdirectories                                                      #
#######################################################################
diskoLibs  = ["mmsinfo",
              "mmstools",
              "mmsconfig",
              "mmsgui",
              "mmsbase",
              "mmsinput",
              "mmscore"]
if env['enable_media']:
	diskoLibs.append("mmsmedia")
if env['enable_flash']:
	diskoLibs.append("mmsflash")
if env['enable_sip']:
	diskoLibs.append("mmssip")
	
if env['enable_tools']:	
	diskoTools = ["taff"]
else:
	diskoTools = ()

#######################################################################
# Helper functions                                                    #
#######################################################################
def tryConfigCommand(context, cmd):
	ret = context.TryAction(cmd)[0]
	context.Result(ret)
	if ret:
		context.env.ParseConfig(cmd)
	return ret

def checkPKGConfig(context):
	context.Message('Checking for pkg-config... ')
	ret = context.TryAction('pkg-config --version')[0]
	context.Result(ret)
	return ret

def checkPKG(context, name):
	return tryConfigCommand(context, 'pkg-config --libs --cflags \'%s\'' % name)

def checkConf(context, name):
	binary = '%s_config' % name.lower()
	configcall = '%s --libs --cflags' %binary
	return tryConfigCommand(context, configcall)

def checkSimpleLib(context, liblist, header = '', lang = 'c++', required = 1):
	for lib in liblist:
		context.Message('Checking for %s... ' % lib)
		ret = checkPKG(context, lib)
		if ret:
			return ret

		ret = checkConf(context, lib)
		if ret:
			return ret

		if len(header):
			ret = conf.CheckLibWithHeader(lib, header, lang)
		else:
			ret = conf.CheckLib(lib)

		if ret:
			#if not lib in conf.env['LIBS']:
			#	conf.env.Append(LIBS = [lib])
			return ret

	if required:
		print 'required lib %s not found :(' % lib
		Exit(1)

	return False

conf = Configure(env, 
                 custom_tests = {'checkPKGConfig' : checkPKGConfig,
                 				 'checkConf': checkConf, 
                 				 'checkPKG': checkPKG, 
                 				 'checkSimpleLib': checkSimpleLib},
                 conf_dir = 'build/.sconf_temp',
                 log_file = 'build/config.log',
                 clean = False,
                 help  = False)

#######################################################################
# Creating pkg-config file                                            #
#######################################################################
def createDiskoPC(env = None):
	disko_pc = open('disko.pc', 'w')
	disko_pc_requires = 'libxml-2.0 >= 2.6, libcurl, sigc++-2.0, libpng >= 1.2'
	disko_pc_libs     = '%s -l%s' % (' -L'.join(env['LIBPATH']).replace('../../../lib',''), ' -l'.join(env['LIBS']))
	
	if 'dfb' in env['graphics']:
		disko_pc_requires += ', directfb'
	  
	if('x11' in env['graphics']):
		disko_pc_requires += ', x11, xv, xxf86vm, freetype2'
		
	if(env['enable_media']):
		if('x11' in env['graphics']):
			disko_pc_requires += ', alsa , libxine >= 1.1.15'
		else:
			disko_pc_requires += ', alsa, libxine'

	if(env['enable_flash']):
		disko_pc_requires += ', swfdec-0.8'

	if(env['enable_sip']):
		disko_pc_requires += ', libpj'
		
	if(env['enable_mail']):
		disko_pc_requires += ', vmime'
		
	disko_pc.write('prefix=' + env['prefix'] + '\n')
	disko_pc.write('exec_prefix=${prefix}\n')
	disko_pc.write('libdir=${exec_prefix}/lib\n')
	disko_pc.write('includedir=${exec_prefix}/include\n\n')
	disko_pc.write('Name: ' + packageRealName + '\n')
	disko_pc.write('Description: ' + packageDescription + '\n')
	disko_pc.write('Version: ' + packageVersion + '\n')
	disko_pc.write('Requires: ' + disko_pc_requires + '\n')
	disko_pc.write('Libs: -L${libdir} ' + disko_pc_libs + '\n')
	disko_pc.write('Cflags: -I${includedir}/ ' + ' '.join(env['CCFLAGS']) + '\n')
	
	disko_pc.close()

#######################################################################
# Check dependencies                                                  #
#######################################################################
def checkDeps(target = None, source = None, env = None):
	# check if graphics backend was chosen
	if not env['graphics']:
		print 'Please choose a graphics backend by using:'
		print '  \'scons graphics=dfb\' or'
		print '  \'scons graphics=x11\' or'
		print '  \'scons graphics=all\'\n'
		Exit(1)

	if not env['database']:
		print 'Please choose a graphics backend by using:'
		print '  \'scons database=sqlite3\' or'
		print '  \'scons database=mysql\' or'
		print '  \'scons database=odbc\'\n'
		Exit(1)

	# checks that are required everytime
	conf.checkPKGConfig()
	conf.checkSimpleLib(['sigc++-2.0'],        'sigc++-2.0/sigc++/sigc++.h')
	conf.checkSimpleLib(['libxml-2.0 >= 2.6'], 'libxml2/libxml/parser.h')
	conf.checkSimpleLib(['libpng >= 1.2'],     'libpng/png.h')
	conf.checkSimpleLib(['libcurl'],           'curl/curl.h')

	# checks required if building DirectFB backend
	if('dfb' in env['graphics']):
		conf.checkSimpleLib(['directfb'],   'directfb/directfb.h')
		env.Append(CCFLAGS = '-D__HAVE_DIRECTFB__')
		
	# checks required if building X11 backend
	if('x11' in env['graphics']):
		conf.checkSimpleLib(['x11'],	   'X11/Xlib.h')
		conf.checkSimpleLib(['xv'],        'X11/extensions/Xvlib.h')
		conf.checkSimpleLib(['xxf86vm'],   'X11/extensions/xf86vmode.h')
		conf.checkSimpleLib(['freetype2'], 'freetype/freetype.h')
		env.Append(CCFLAGS = '-D__HAVE_XLIB__ -D__ENABLE_MMSFB_X11_CORE__ -D__ENABLE_MMSFBSURFACE_X11_CORE__')
		
	# checks required if building mmsmedia
	if(env['enable_media']):
		if('x11' in env['graphics']):
			conf.checkSimpleLib(['libxine >= 1.1.15'],    'xine.h')
		else:
			conf.checkSimpleLib(['libxine'],    'xine.h')
		conf.checkSimpleLib(['alsa'],       'alsa/version.h')
	else:
		env.Append(CCFLAGS = '-D_NO_MMSMEDIA -D_NO_MIXER')
		
	# checks required for database backends
	dbs = []
	mmscrypt = 0
	
	if 'sqlite3' in env['database']:
		conf.checkSimpleLib(['sqlite3'],    'sqlite3.h')
		env.Append(CCFLAGS = '-D__ENABLE_SQLITE__')
	if 'mysql' in env['database']:
		conf.checkSimpleLib(['mysql'],      'mysql.h')
		env.Append(CCFLAGS = '-D__ENABLE_MYSQL__')
	if 'odbc' in env['database']:
		if conf.CheckCXXHeader('/usr/include/sql.h'):
			env.Append(CCFLAGS = '-D__ENABLE_FREETDS__', LIBS = 'odbc')
			dbs.append('odbc')
		elif conf.CheckCXXHeader('/usr/local/include/sql.h'):
			env.Append(CCFLAGS = '-D__ENABLE_FREETDS__ -I/usr/local/include', LIBPATH = '/usr/local/lib', LIBS = 'odbc')
			dbs.append('odbc')
		else:
			Exit(1)
	
	# check for openssl
	if not conf.checkSimpleLib(['openssl'],    'openssl/conf.h', required = 0):
		env.Append(CCFLAGS = '-D_NO_MMSCRYPT')
	else:
		mmscrypt = 1
		
	# checks required if building mmsflash
	if(env['enable_flash']):
		conf.checkSimpleLib(['swfdec-0.8'], 'swfdec-0.8/swfdec/swfdec.h')
	else:
		env.Append(CCFLAGS = '-D_NO_MMSFLASH')
	
	# checks required if building mmssip
	if(env['enable_sip']):
		conf.checkSimpleLib(['libpj'], 'pjlib.h')
	else:
		env.Append(CCFLAGS = '-D_NO_MMSSIP')
		
	# checks required if building with email support
	if(env['enable_mail']):
		conf.checkSimpleLib(['vmime'], 'vmime.h')
		env.Append(CCFLAGS = '-D__HAVE_VMIME__')

	env = conf.Finish()
		
	print '\n********************* Summary *********************\n'
	print 'Graphic backends  : %s' % env['graphics']
	print 'Database backends : %s\n' % ', '.join(env['database'])
	if(mmscrypt == 1):
		print 'Building mmscrypt : yes'
	else:
		print 'Building mmscrypt : no'
	if(env['enable_flash']):
		print 'Building mmsflash : yes'
	else:
		print 'Building mmsflash : no'
	if(env['enable_sip']):
		print 'Building mmssip   : yes'
	else:
		print 'Building mmssip   : no'
	if(env['enable_mail']):
		print 'E-Mail support    : yes'
	else:
		print 'E-Mail support    : no'
	if(env['enable_tools']):
		print 'Building tools    : yes\n'
	else:
		print 'Building tools    : no\n'
	if(env['messages']):
		print 'log messages      : yes'
	else:
		print 'log messages      : no'
	if(env['debug']):
		print 'debug symbols     : yes'
	else:
		print 'debug symbols     : no'
	if(env['profile']):
		print 'profiling info    : yes'
	else:
		print 'profiling info    : no'
	if(env['use_sse']):
		print 'SSE optimization  : yes'
	else:
		print 'SSE optimization  : no'
	print '\n***************************************************\n'
	
	createDiskoPC(env)

#######################################################################
# Building disko                                                      #
#######################################################################
libList = ""
for libDir in diskoLibs:
        libList += 'build/libs/' + libDir + "/SConscript "

toolList = ""
for toolDir in diskoTools:
        toolList += 'build/tools/' + toolDir + "/SConscript "

BuildDir('build/libs', 'src', duplicate = 0)
SConscript(Split(libList), options = opts)

BuildDir('build/tools', 'tools', duplicate = 0)
SConscript(Split(toolList), options = opts)

#######################################################################
# Create targets                                                      #
#######################################################################
env.Decider('MD5-timestamp')
check = env.Alias(target = 'check', 
                  action = Action(checkDeps, "\nChecking for dependencies:\n"))
env.AlwaysBuild(check)
all = env.Alias('all', [check, 'lib', 'bin'])
install = env.Alias('install', [check, idir_prefix])
Depends(install, all)
env.Default(all)
env.Install(idir_prefix + '/lib/pkgconfig', 'disko.pc')
Clean('lib', 'disko.pc')

#######################################################################
#  Documentation                                                      #
#######################################################################
doxygenBuilder = Builder(action = 'doxygen $SOURCE')
env.Append(BUILDERS = { 'DoxygenDoc' : doxygenBuilder })
doxygenDocPath = '(doc)'
env.DoxygenDoc(doxygenDocPath, 'doc/conf/disko.conf')
env.Alias('doc', doxygenDocPath)