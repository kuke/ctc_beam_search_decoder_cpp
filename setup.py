from distutils.core import setup, Extension
from os import system

ctc_beam_search_decoder_module = Extension('_ctc_beam_search_decoder',
                           sources=['ctc_beam_search_decoder_wrap.cxx', 'ctc_beam_search_decoder.cpp', 'scorer_wrap.cxx', 'scorer.cpp'],
                          )

system('swig -python -c++ ./ctc_beam_search_decoder.i')
setup(name='ctc_beam_search_decoder',
      version='0.1',
      author='Yibing Liu',
      description="""CTC beam search decoder""",
      ext_modules=[ctc_beam_search_decoder_module],
      py_modules=['ctc_beam_search_decoder'],
      )

swig_scorer_module = Extension('_scorer',

                           sources=['scorer_wrap.cxx', 'scorer.cpp'],
                          )

system('swig -python -c++ ./scorer.i')
setup(name='scorer',
      version='0.1',
      author='Yibing Liu',
      description="""External defined scorer for beam search decoder""",
      ext_modules=[swig_scorer_module],
      py_modules=['scorer'],
      )
