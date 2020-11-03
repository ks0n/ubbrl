project('ubbrl', 'c',
  default_options: [
    'warning_level=3',
    'debug=true',
    'b_sanitize=address',
    'c_std=c99',
    ])

add_project_arguments('-D_POSIX_C_SOURCE=200809L', language: 'c')

inc = include_directories(['include', 'src'])

src = [
  'src/charstream.c',
  'src/ubbrl.c',
  'src/wordvec.c',
  'src/vec.c',
  'src/history.c',
  ]

main = [ 'src/main.c' ]

library('ubbrl', src, include_directories: inc)

executable('ubbrl', src, main, include_directories: inc)

unit_tests_src = [
  'tests/test-ubbrl.c',
  'tests/charstream.c',
  'tests/wordvec.c',
  ]

criterion = dependency('criterion')

unit_tests = executable('ubbrl_unit_tests',
  src,
  unit_tests_src,
  dependencies: [criterion],
  include_directories: inc)

test('libUBBRL Unit Tests', unit_tests)
