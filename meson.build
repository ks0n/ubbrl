# FIXME
project('ubbrl', 'c',
  default_options: [
    'warning_level=3',
    'debug=true',
    'b_sanitize=address',
    'c_std=c99',
    ])

add_project_arguments(['-D_POSIX_C_SOURCE=200809L', '-D_GNU_SOURCE'], language: 'c')

inc = ['include', 'src', 'subprojects/c_omponents/include']

c_omponents = subproject('c_omponents')
c_omponents_dep = c_omponents.get_variable('c_omponents_dep')

inc_dirs = include_directories(inc)

src = [
  'src/charstream.c',
  'src/ubbrl.c',
  'src/wordvec.c',
  'src/history.c',
  ]

main = [ 'src/main.c' ]

ubbrl_so = library('ubbrl', src, include_directories: inc, link_with: c_omponents_dep)
executable('ubbrl', main, include_directories: inc, link_with: ubbrl_so)

unit_tests_src = [
  'tests/test-ubbrl.c',
  'tests/charstream.c',
  'tests/wordvec.c',
  ]

criterion = dependency('criterion')

unit_tests = executable('ubbrl_unit_tests',
  src,
  unit_tests_src,
  link_with: [ubbrl_so, c_omponents_dep],
  dependencies: [criterion],
  include_directories: inc)

test('libUBBRL Unit Tests', unit_tests)
