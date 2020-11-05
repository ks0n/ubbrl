# FIXME
project('ubbrl', 'c',
  default_options: [
    'warning_level=3',
    'debug=true',
    'b_sanitize=address',
    'c_std=c99',
    ])

add_project_arguments('-D_POSIX_C_SOURCE=200809L', language: 'c')

inc = include_directories(['include', 'src', 'lib/c_omponents/include']) # FIXME: Ugly

subdir('lib')

deps = [
  c_omponents_dep,
  ]

src = [
  'src/charstream.c',
  'src/ubbrl.c',
  'src/wordvec.c',
  'src/history.c',
  ]

main = [ 'src/main.c' ]

ubbrl_so = library('ubbrl', src, include_directories: inc, dependencies: deps)
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
  link_with: ubbrl_so,
  dependencies: [deps, criterion],
  include_directories: inc)

test('libUBBRL Unit Tests', unit_tests)
