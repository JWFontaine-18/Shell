# Shell

A minimal shell supporting command execution, limited piping, redirection and background proccessing. 

## Group Members
- **Hugh Long**: hal20a@fsu.edu
- **James Fontaine**: jwf22c@fsu.edu
- **Alex Brown**: ab19@fsu.edu
## Division of Labor

### Part 1: Prompt
- **Responsibilities**: [Greet user with prompt]
- **Assigned to**: James Fontaine

### Part 2: Environment Variables
- **Responsibilities**: [Expand env variable by replacing tokens prefixed with '$']
- **Assigned to**: James Fontaine

### Part 3: Tilde Expansion
- **Responsibilities**: [Description]
- **Assigned to**: Alex Brown

### Part 4: $PATH Search
- **Responsibilities**: [Description]
- **Assigned to**: Alex Brown, Jane Smith

### Part 5: External Command Execution
- **Responsibilities**: [Description]
- **Assigned to**: Steven Kester, James Fontaine

### Part 6: I/O Redirection
- **Responsibilities**:All: a couple of functions support io-redirection, expects only one command
- **Assigned to**: Hugh Long, James Fontaine

### Part 7: Piping
- **Responsibilities**: All: piping only supports 2 pipes/3 commands per proj description
- **Assigned to**: Hugh Long

### Part 8: Background Processing
- **Responsibilities**: All: typedef with mutator functions enables keeping track of background processes
- **Assigned to**: Hugh Long, James Fontaine

### Part 9: Internal Command Execution
- **Responsibilities**: [Description]
- **Assigned to**: Alex Brown

### Part 10: External Timeout Executable
- **Responsibilities**: [Description]
- **Assigned to**: Alex Brown, Jane Smith

### Extra Credit
- **Responsibilities**: [Description]
- **Assigned to**: Hugh Long 

## File Listing
```
shell/
│
├── src/
│ ├── background.c
│ └── command_utils.c
| └── externalcomands.c
| └── ioredirect.c
| └── lexer.c
| └── path_search.c
| └── piping.c
| └── prompt.c
| └── 
│
├── include/
│ └── piping.h
| └── background.h
| └── command_utils.h
| └── externalcommand.h
| └── lexer.h
| └── path_search.h
│ └── piping.h
| └── prompt.h
| └── redirect.h
├── README.md
|── .gitignore
└── Makefile
```
## How to Compile & Execute

### Requirements
- **Compiler**: gcc.
- **Dependencies**: none

### Compilation
```bash
make
```
This will build the executable in ... /bin/shell
### Execution
```bash
make run
```
This will run the program ...
./bin/shell or make run
## Development Log

### Hugh Long

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2025-09-16 | part 6 implement  |
| 2025-09-18 | part 7 implement  |
| 2025-09-22 | part 8 implement  |
| 2025-09-25 | mem leak fix on error    |   
                          
### Steven Kester

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2025-09-12 | part 3 implement  |
| YYYY-MM-DD | [Description of task]  |
| YYYY-MM-DD | [Description of task]  |


### James Fontaine

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2025-09-10 | Part 1/ 2 Implement  |
| 2025-09-15 | PArt 6 Implementation |
| 2025-09-22 | Part 8 Started implementation |


## Meetings
Document in-person meetings, their purpose, and what was discussed.

| Date       | Attendees            | Topics Discussed | Outcomes / Decisions |
|------------|----------------------|------------------|-----------------------|
| 2025-09-08 | All              | work assignments   | turn in work division  |
| 2025-09-15 | checkup              | begin work/check status   | complete work assigned, keep in contact over discord  |



## Bugs
- **Bug 1**: This is bug 1.
- **Bug 2**: This is bug 2.
- **Bug 3**: This is bug 3.

## Extra Credit
- **Extra Credit 1**: [Extra Credit Option]
- **Extra Credit 2**: [Extra Credit Option]
- **Extra Credit 3**: [Extra Credit Option]

## Considerations
[Description]
