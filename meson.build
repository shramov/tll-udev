project('tll-udev', 'c', 'cpp', version: '0.0.0', default_options: ['cpp_std=c++17', 'werror=true', 'optimization=2'])

include = [include_directories('src')]

tll = dependency('tll')
fmt = dependency('fmt')
udev = dependency('libudev')

lib = shared_library('tll-udev',
	['src/channel.cc'],
	include_directories : include,
	dependencies : [fmt, tll, udev],
	install : true,
)
