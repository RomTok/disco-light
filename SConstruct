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

import os, sys, string, re, SCons

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
packageVersionRC    = ''

# Package information
packageName        = 'disko'
packageRealName    = 'Disko Framework'
packageDescription = 'Disko application framework for embedded devices (http://www.diskohq.org)'
packageVersion     = '%d.%d.%d%s' % (packageVersionMajor, packageVersionMinor, packageVersionMicro, packageVersionRC)

#######################################################################
# Get SCons version (copied from internal scons function)             #
#######################################################################
def GetSconsVersion():
	"""Split a version string into major, minor and (optionally)
	   revision parts.   
	   This is complicated by the fact that a version string can be
	   something like 3.2b1."""
	   
	version = string.split(string.split(SCons.__version__, ' ')[0], '.')
	v_major = int(version[0])
	v_minor = int(re.match('\d+', version[1]).group())
	if len(version) >= 3:
		v_revision = int(re.match('\d+', version[2]).group())
	else:
		v_revision = 0
		
	return v_major, v_minor, v_revision

sconsVersion = GetSconsVersion()

#######################################################################
# Help text                                                           #
#######################################################################
Help("Type: 'scons [options]' to build disko.\n" +
     "      'scons [options] check' to check the requirements for building disko.\n" +
     "      'scons -c' to clean.\n" +
     "      'scons doc' to create the API reference (doxygen has to be installed).\n" +
     "      'scons install' to install disko.\n\n" +
     "The following options are available:\n")



#######################################################################
# Helper functions                                                    #
#######################################################################
def PathIsDirCreateNone(key, value, env):
	if(value != 'none'):
		if sconsVersion < (0,98,1):
			PathOption.PathIsDirCreate(key, value, env)
		else:
			PathVariable.PathIsDirCreate(key, value, env)

#######################################################################
# Command line options                                                #
#######################################################################
if sconsVersion < (0,98,1):
	opts = Options('disko.conf')
	opts.AddOptions(
    	PathOption('prefix',        'Installation directory', '/usr', PathOption.PathIsDirCreate),
    	PathOption('destdir',       'Installation directory for cross-compile', 'none', PathIsDirCreateNone),
    	BoolOption('debug',         'Build with debug symbols and without optimize', False),
    	BoolOption('messages',      'Build with logfile support', False),
    	BoolOption('profile',       'Build with profiling support (includes debug option)', False),
    	BoolOption('use_sse',       'Use SSE optimization', False),
    	ListOption('graphics',      'Set graphics backend', 'none', ['dfb', 'x11']),
    	ListOption('database',      'Set database backend', 'sqlite3', ['sqlite3', 'mysql', 'odbc']),
    	BoolOption('enable_media',  'Build with mmsmedia support', True),
    	BoolOption('enable_flash',  'Build with mmsflash support', False),
    	BoolOption('enable_sip',    'Build with mmssip support', False),
    	BoolOption('enable_mail',   'Build with email support', False),
    	BoolOption('enable_tools',  'Build disko tools', False),
    	BoolOption('enable_static', 'Create statically linked library', False))
else:
	opts = Variables('disko.conf')
	opts.AddVariables(
    	PathVariable('prefix',        'Installation directory', '/usr', PathVariable.PathIsDirCreate),
    	PathVariable('destdir',       'Installation directory for cross-compile', 'none', PathIsDirCreateNone),
    	BoolVariable('debug',         'Build with debug symbols and without optimize', False),
    	BoolVariable('messages',      'Build with logfile support', False),
    	BoolVariable('profile',       'Build with profiling support (includes debug option)', False),
    	BoolVariable('use_sse',       'Use SSE optimization', False),
    	ListVariable('graphics',      'Set graphics backend', 'none', ['dfb', 'x11']),
    	ListVariable('database',      'Set database backend', 'sqlite3', ['sqlite3', 'mysql', 'odbc']),
    	BoolVariable('enable_media',  'Build with mmsmedia support', True),
    	BoolVariable('enable_flash',  'Build with mmsflash support', False),
    	BoolVariable('enable_sip',    'Build with mmssip support', False),
    	BoolVariable('enable_mail',   'Build with email support', False),
    	BoolVariable('enable_tools',  'Build disko tools', False),
    	BoolVariable('enable_static', 'Create statically linked library', False))

env = Environment(ENV = os.environ, CPPPATH = os.getcwd() + '/inc')

# use environment variables to override defaults
if os.environ.has_key('CXX'):
	env['CXX'] = os.environ['CXX'] 
if os.environ.has_key('LD'):
	env['LINK'] = os.environ['LD']
	 
env['LIBPATH'] = ''
env['diskoSources'] = []

opts.Update(env)
opts.Save('disko.conf', env)
Help(opts.GenerateHelpText(env))

# Here are our installation paths:
if env['destdir'] != 'none':
	idir_prefix = env['destdir']
else:
	idir_prefix = env['prefix']

idir_lib    = idir_prefix + '/lib/disko'
idir_bin    = idir_prefix + '/bin'
idir_inc    = idir_prefix + '/include/disko'
idir_data   = idir_prefix + '/share/disko'

# link with -rpath
env.Append(LDFLAGS = '-Wl,-rpath=' + env['prefix'] + '/lib/disko')

# extra flags
if env['messages']:
	env['CCFLAGS'].append('-D__ENABLE_LOG__')

if env['profile']:
	env['CCFLAGS'].append('-pg')
	env.Replace(debug = 1)

if env['debug']:
	env['CCFLAGS'].append(['-O0', '-g', '-Wall', '-D__ENABLE_DEBUG__'])
else:
	env['CCFLAGS'].append('-O3')

if env['use_sse']:
	env['CCFLAGS'].append(['-msse2', '-mfpmath=sse', '-D__HAVE_SSE__'])

# format output
env['SHCXXCOMSTR']  = '  [CXX] $SOURCE'
env['SHLINKCOMSTR'] = '  [LD]  $TARGET'
env['CXXCOMSTR']    = '  [CXX] $SOURCE'
env['LINKCOMSTR']   = '  [LD]  $TARGET'

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
def checkOptions(context):
	context.Message('Checking for options...')
	# check if graphics backend was chosen
	if not env['graphics']:
		print '\nPlease choose a graphics backend by using:'
		print '  \'scons graphics=dfb\' or'
		print '  \'scons graphics=x11\' or'
		print '  \'scons graphics=all\'\n'
		Exit(1)

	if not env['database']:
		print '\nPlease choose a graphics backend by using:'
		print '  \'scons database=sqlite3\' or'
		print '  \'scons database=mysql\' or'
		print '  \'scons database=odbc\'\n'
		Exit(1)
		
	# to avoid 'error: no result' msg
	context.Result(True)
	
	return True

def tryConfigCommand(context, cmd):
	ret = context.TryAction(cmd)[0]
	
	if ret:
		try:
			context.env.ParseConfig(cmd)
		except OSError:
			ret = 0
	return ret

def checkPKGConfig(context):
	context.Message('Checking for pkg-config... ')
	ret = context.TryAction('pkg-config --version')[0]
	context.Result(ret)
	return ret

def checkPKG(context, name):
	return tryConfigCommand(context, 'pkg-config --libs --cflags \'%s\'' % name)

def checkConf(context, name):
	seperators = ['-', '_']
	for sep in seperators:
		configcall = '%s%sconfig --libs --cflags' % (name.lower(), sep)
		if(tryConfigCommand(context, configcall)):
			return True
	return False

def checkSimpleLib(context, liblist, header = '', lang = 'c++', required = 1):
	for lib in liblist:
		context.Message('Checking for %s... ' % lib)
		ret = checkPKG(context, lib)
		if ret:
			context.Result(True)
			return True

		ret = checkConf(context, lib)
		if ret:
			context.Result(True)
			return True

		# redirect stdout to suppress messages from built in checks
		sys.stdout = open('/dev/null', 'a')
		if len(header):
			ret = conf.CheckLibWithHeader(liblist, header, lang)
		else:
			ret = conf.CheckLib(lib)
		# restore stdout
		sys.stdout.close()
		sys.stdout = sys.__stdout__
		if ret:
			#if not lib in conf.env['LIBS']:
			#	conf.env.Append(LIBS = [lib])
			context.Result(True)
			return True

	context.Result(False)

	if required:
		print 'required lib %s not found' % lib
		Exit(1)

	return False
		
def printSummary():
	print '\n********************* Summary *********************\n'
	print 'Prefix:           : %s' % conf.env['prefix']
	print 'Destdir:          : %s' % conf.env['destdir']
	print 'Graphic backends  : %s' % conf.env['graphics']
	print 'Database backends : %s\n' % ', '.join(conf.env['database'])
	if(conf.env['mmscrypt']):
		print 'Building mmscrypt : yes'
	else:
		print 'Building mmscrypt : no'
	if(conf.env['enable_media']):
		print 'Building mmsmedia : yes'
	else:
		print 'Building mmsmedia : no'
	if(conf.env['enable_flash']):
		print 'Building mmsflash : yes'
	else:
		print 'Building mmsflash : no'
	if(conf.env['enable_sip']):
		print 'Building mmssip   : yes'
	else:
		print 'Building mmssip   : no'
	if(conf.env['enable_mail']):
		print 'E-Mail support    : yes'
	else:
		print 'E-Mail support    : no'
	if(conf.env['enable_tools']):
		print 'Building tools    : yes\n'
	else:
		print 'Building tools    : no\n'
	if(conf.env['messages']):
		print 'log messages      : yes'
	else:
		print 'log messages      : no'
	if(conf.env['debug']):
		print 'debug symbols     : yes'
	else:
		print 'debug symbols     : no'
	if(conf.env['profile']):
		print 'profiling info    : yes'
	else:
		print 'profiling info    : no'
	if(conf.env['use_sse']):
		print 'SSE optimization  : yes'
	else:
		print 'SSE optimization  : no'
	print '\n***************************************************\n'

#######################################################################
# Check dependencies                                                  #
#######################################################################
conf = Configure(env,
                 custom_tests = {'checkOptions' : checkOptions,
                 				 'checkPKGConfig' : checkPKGConfig,
                 				 'checkConf' : checkConf,
                 				 'checkPKG' : checkPKG,
                 				 'checkSimpleLib' : checkSimpleLib},
                 conf_dir = 'build/.sconf_temp',
                 log_file = '/dev/null',
                 clean = False,
                 help  = False)

conf.checkOptions()

# checks that are required everytime
conf.checkPKGConfig()
conf.checkSimpleLib(['sigc++-2.0'],        'sigc++-2.0/sigc++/sigc++.h')
conf.checkSimpleLib(['libxml-2.0 >= 2.6'], 'libxml2/libxml/parser.h')
conf.checkSimpleLib(['libpng >= 1.2'],     'libpng/png.h')
conf.checkSimpleLib(['libcurl'],           'curl/curl.h')
conf.checkSimpleLib(['freetype2'], 'freetype/freetype.h')

# checks required if building DirectFB backend
if('dfb' in env['graphics']):
	conf.checkSimpleLib(['directfb'],   'directfb/directfb.h')
	conf.env['CCFLAGS'].append('-D__HAVE_DIRECTFB__')
	
# checks required if building X11 backend
if('x11' in env['graphics']):
	conf.checkSimpleLib(['x11'],	   'X11/Xlib.h')
	conf.checkSimpleLib(['xv'],        'X11/extensions/Xvlib.h')
	conf.checkSimpleLib(['xxf86vm'],   'X11/extensions/xf86vmode.h')
	conf.env['CCFLAGS'].append(['-D__HAVE_XLIB__',
				'-D__ENABLE_MMSFB_X11_CORE__',
				'-D__ENABLE_MMSFBSURFACE_X11_CORE__'])
	
# checks required if building mmsmedia
if(env['enable_media']):
	conf.env['CCFLAGS'].append(['-DXINE_DISABLE_DEPRECATED_FEATURES'])
	if('x11' in env['graphics']):
		conf.checkSimpleLib(['libxine >= 1.1.15'],    'xine.h')
	else:
		conf.checkSimpleLib(['libxine'],    'xine.h')
	conf.checkSimpleLib(['alsa'],       'alsa/version.h')
else:
	conf.env['CCFLAGS'].append(['-D_NO_MMSMEDIA', '-D_NO_MIXER'])
	
	
# checks required for database backends
if 'sqlite3' in env['database']:
	conf.checkSimpleLib(['sqlite3', 'pthread'],    'sqlite3.h')
	conf.env['CCFLAGS'].append('-D__ENABLE_SQLITE__')
if 'mysql' in env['database']:
	conf.checkSimpleLib(['mysql'],      'mysql.h')
	conf.env['CCFLAGS'].append('-D__ENABLE_MYSQL__')
if 'odbc' in env['database']:
	if conf.CheckCXXHeader('/usr/include/sql.h'):
		conf.env.Append(LIBS = 'odbc')
		conf.env['CCFLAGS'].append('-D__ENABLE_FREETDS__')
	elif conf.CheckCXXHeader('/usr/local/include/sql.h'):
		conf.env.Append(LIBPATH = '/usr/local/lib', LIBS = 'odbc')
		conf.env['CCFLAGS'].append(['-D__ENABLE_FREETDS__', '-I/usr/local/include'])
	else:
		Exit(1)

# check for openssl
if not conf.checkSimpleLib(['openssl'],    'openssl/conf.h', required = 0):
	conf.env['CCFLAGS'].append('-D_NO_MMSCRYPT')
	conf.env['mmscrypt'] = 0
else:
	conf.env['mmscrypt'] = 1
	
# checks required if building mmsflash
if(env['enable_flash']):
	conf.checkSimpleLib(['swfdec-0.8'], 'swfdec-0.8/swfdec/swfdec.h')
else:
	conf.env['CCFLAGS'].append('-D_NO_MMSFLASH')

# checks required if building mmssip
if(env['enable_sip']):
	if conf.checkSimpleLib(['libpj'], 'pjlib.h'):
		conf.checkSimpleLib(['uuid'], 'uuid/uuid.h', required = 0)	
else:
	conf.env['CCFLAGS'].append('-D_NO_MMSSIP')
	
# checks required if building with email support
if(env['enable_mail']):
	conf.checkSimpleLib(['vmime'], 'vmime.h')
	conf.env['CCFLAGS'].append('-D__HAVE_VMIME__')

env2 = conf.Finish()
if env2:
	env = env2
	printSummary()
	
if 'check' in BUILD_TARGETS:
	Exit(0)

#######################################################################
# Creating pkg-config file                                            #
#######################################################################
# TODO: handle disko_pc_libs                                          #
if 'install' in BUILD_TARGETS:
	disko_pc = open('disko.pc', 'w')
	disko_pc_requires = 'libxml-2.0 >= 2.6, libcurl, sigc++-2.0, libpng >= 1.2, freetype2'
	if(env['LIBPATH']):
		disko_pc_libs     = '-L%s' % ' -L'.join(env['LIBPATH'])
	else:
		disko_pc_libs = ''
	disko_pc_libs    += ' -lmmsinfo -lmmsconfig -lmmstools -lmmsgui -lmmsinput -lmmsbase -lmmscore'
	
	if 'dfb' in env['graphics']:
		disko_pc_requires += ', directfb'
	  
	if('x11' in env['graphics']):
		disko_pc_requires += ', x11, xv, xxf86vm'
		
	if(env['enable_media']):
		disko_pc_libs += ' -lmmsmedia'
		if('x11' in env['graphics']):
			disko_pc_requires += ', alsa , libxine >= 1.1.15'
		else:
			disko_pc_requires += ', alsa, libxine'

	if(env['enable_flash']):
		disko_pc_requires += ', swfdec-0.8'
		disko_pc_libs += ' -lmmsflash'

	if(env['enable_sip']):
		disko_pc_requires += ', libpj'
		disko_pc_libs += ' -lmmssip'
		if('uuid' in env['LIBS']):
			disko_pc_requires += ', uuid'
		
	if(env['enable_mail']):
		disko_pc_requires += ', vmime'
		
	if(env['mmscrypt']):
		disko_pc_requires += ', openssl'

	if 'sqlite3' in env['database']:
		disko_pc_requires += ', sqlite3'
		
	if 'mysql' in env['database']:
		disko_pc_requires += ', mysql'

	disko_pc.write('prefix=' + env['prefix'] + '\n')
	disko_pc.write('exec_prefix=${prefix}\n')
	disko_pc.write('libdir=${exec_prefix}/lib/disko\n')
	disko_pc.write('includedir=${exec_prefix}/include/disko\n\n')
	disko_pc.write('Name: ' + packageRealName + '\n')
	disko_pc.write('Description: ' + packageDescription + '\n')
	disko_pc.write('Version: ' + packageVersion + '\n')
	disko_pc.write('Requires: ' + disko_pc_requires + '\n')
	disko_pc.write('Libs: -L${libdir} ' + disko_pc_libs + '\n')
	disko_pc.write('Cflags: -I${includedir}/ ')
	for ccflag in env['CCFLAGS']:
		disko_pc.write(' ')
		if(type(ccflag).__name__ == 'str'):
			disko_pc.write(ccflag)
		else:
			disko_pc.write(' '.join(ccflag))
	disko_pc.write('\n')
	
	disko_pc.close()

#######################################################################
# Create targets                                                      #
#######################################################################
env['TOP_DIR'] = os.getcwd()
env.Decider('MD5-timestamp')
if(env['enable_tools']):
	all = env.Alias('all', ['lib', 'bin'])
else:
	all = env.Alias('all', ['lib'])
install = env.Alias('install', [idir_prefix])
Depends(install, all)
env.Default(all)	

env.Install(idir_inc, env['TOP_DIR'] + '/inc/mms.h')
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

#######################################################################
# Building disko                                                      #
#######################################################################
Export('env idir_lib idir_bin idir_inc idir_data')

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
# Static Library                                                      #
#######################################################################
if env['enable_static']:
	libdisko_static = env.StaticLibrary('lib/libdisko', env['diskoSources'])
	env.Install(idir_lib, libdisko_static)
