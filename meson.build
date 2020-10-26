project('ubbrl', 'c',
  default_options: [
    'warning_level=3',
    'debug=true',
    'b_sanitize=address',
    'c_std=c99',
    ])

inc = include_directories('include')

src = [
  'src/ubbrl.c',
  'src/vec.c',
  'src/history.c',
  ]

library('ubbrl', src, include_directories: inc)

unit_tests_src = [
  'tests/test-ubbrl.c',
  ]

criterion = dependency('criterion')

unit_tests = executable('ubbrl_unit_tests',
  src,
  unit_tests_src,
  dependencies: [criterion],
  include_directories: inc)

test('libUBBRL Unit Tests', unit_tests)
