import doctest
import inspect
import low_index
import types
from doctest import DocTestFinder, DocTestParser, DocTestRunner

modules = [low_index]

def fix_module_doctest(module):
    """
    Extract docstrings from cython functions that would be skipped by doctest
    otherwise.
    """
    module.__test__ = {}
    for name in dir(module):
        value = getattr(module, name)
        # Avoid duplicate tests for python classes.
        try:
            if value.__module__ == module.__name__:
                continue
        except AttributeError:
            pass
        if inspect.isclass(value) or inspect.isfunction(value):
            if value.__doc__ and isinstance(value.__doc__, str):
                module.__test__[name] = value.__doc__

def doctest_module(module, verbose=False):
    finder = doctest.DocTestFinder(parser=DocTestParser())
    if isinstance(module, types.ModuleType):
        fix_module_doctest(module)
        runner = doctest.DocTestRunner(verbose=verbose)
        for test in finder.find(module):
            runner.run(test)
        result = runner.summarize()
    else:
        result = module(verbose=verbose)
    return result

def runtests(verbose=False):
    success = True
    for module in modules:
        result = doctest_module(module, verbose=verbose)
        if result.failed > 0:
            success = False
        print(module.__name__ + ':', result)

    return success

if __name__ == '__main__':
    import sys
    verbose = '-v' in sys.argv or '--verbose' in sys.argv
    if not runtests(verbose):
        sys.exit(1)
