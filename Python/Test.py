import sys

def main():
    """Script main program."""
    print("Hello World")
    
    a=[0,1]
    print(a.index(0))
    
    mylist = "for statement"
    for word in mylist:
        print(word)
    else:
        print("End list")

    if a is [0,1]:
        print("Equal!")
    elif a is [0,2]:
        print("0,2")
    else:
        print("Else")
    
        
    
    return 0
    
if __name__ == "__main__":
    exit_status = int(not main())
    sys.exit(exit_status)