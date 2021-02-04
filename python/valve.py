import re
import os

def parse(str):
    out = []
    
    in_comment = False
    
    str_open = False
    key_str = ""
    key_string_done = False
    
    value_str = ""
    value_string_done = False
    value_is_dict = False
    value_brace_depth = 0
    
    last_c = ''
    for c in str:
        if in_comment:
            if c == "\n":
                in_comment = False
        elif c == "/" and not str_open:
            if last_c == "/":
                in_comment = True
        elif c == '"' and not (str_open and last_c == "\\"):
            if str_open: # string just closed
                if not key_string_done:
                    key_string_done = True
                elif value_is_dict:
                    value_str += c
                elif not value_string_done: # value was a string, and is now done
                    out.append((key_str, value_str))
                    value_str = ""
                    key_str = ""
                    value_string_done = False
                    key_string_done = False
            else: # string just opened
                if not key_string_done: # we will now begin recording key string
                    pass
                elif not value_is_dict: # value is a string
                    pass
                else: # value is a dict
                    value_str += c
            str_open = not str_open    
        elif str_open: # we are inside a string
            if not key_string_done:
                key_str += c
            elif not value_string_done:
                value_str += c
        elif key_string_done and c == "{":
            if value_brace_depth > 0: # this is a nested brace
                value_str += c
            value_brace_depth += 1
            value_is_dict = True
        elif key_string_done and value_is_dict and c == "}":
            value_brace_depth -= 1
            if value_brace_depth == 0: # this was the last brace, value_str has closed
                out.append((key_str, parse(value_str)))
                value_str = ""
                key_str = ""
                value_string_done = False
                key_string_done = False
                value_is_dict = False
            else:
                value_str += c
        last_c = c
    return out

def filename(x):
    return ''.join([y for y in x if (y in "._" or y.isalnum())])

def __export(items, base, n, N):
    if not os.path.exists(base):
        os.mkdir(base)
    for (name, value) in items:
        if isinstance(value, list):
            n = __export(value, os.path.join(base, filename(name + "/")), n, N)
        elif isinstance(value, str):
            with open(os.path.join(base, filename(name + ".txt")), "wb+") as f:
                f.write(value.encode("utf-16"))
            n += 1
            if n % 25 == 0:
                print("%.2f%% done" % (100*n/N), end = "\t\t\r")
    return n

def __count(items):
    N = 0
    for (name, value) in items:
        if isinstance(value, list):
            N += __count(value)
        elif isinstance(value, str):
            N += 1
    return N

def export(items, base = "export/"):
    N = __count(items)
    print("files to be created: %s" % N)
    __export(items, base, 0, N)
    
    
    
    
    
    
    
    
    
    
    
    
    
    