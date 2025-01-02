import argparse
import json
from types import SimpleNamespace


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('json_file', type=argparse.FileType('r'))
    parser.add_argument('args', nargs='*')
    args = parser.parse_args()
    config = json.load(args.json_file, object_hook=lambda d: SimpleNamespace(**d))
    args = args.args
    pure_args: tuple[str, ...] = tuple(''.join(filter(str.isalpha, the_arg.casefold())) for the_arg in args)
    run_mode = 'run' == pure_args[0] or 'run' in pure_args
    debug_mode = 'debug' == pure_args[0] or 'debug' in pure_args
    if hasattr(config, 'bridgeParams') and hasattr(config.bridgeParams, 'altirraExecutable'):
        cmds = "{' '.join(args)}"
        if hasattr(config.bridgeParams, 'altirraArgumentTemplate'):
            cmds = config.bridgeParams.altirraArgumentTemplate
        if debug_mode and hasattr(config.bridgeParams, 'altirraDebugArgumentTemplate'):
            cmds = config.bridgeParams.altirraDebugArgumentTemplate
        elif run_mode and hasattr(config.bridgeParams, 'altirraRunArgumentTemplate'):
            cmds = config.bridgeParams.altirraRunArgumentTemplate
        command_params = eval(f'f"""{cmds}"""')
        print(f'"{config.bridgeParams.altirraExecutable}" {command_params}')


if __name__ == '__main__':
    main()
