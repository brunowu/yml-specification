![YML logo](YML_logo.png)
# Yml Specification

YML is a research project that aims at providing tools for using global computing middleware such as GRID, Peer to Peer and metacomputing environment. YML software architecture enables the definition of parallel application independently of the underlying middleware used. Parallel applications are defined using a workflow language called YvetteML. You can find more information in [YML offical site](http://yml.prism.uvsq.fr/index.php?lang=fr&page=0).

##TUTORIAL 1: First application                 

Follow the tutorial and the TODO points.

This is a short tutorial to help you getting started with YML.  It
requires you already configure, build and install YML. This tutorial
does not make use of the manager. We will make use of the following
program:
        - yml_component: the component generator
        - yml_compiler: the application compiler
        - yml_scheduler: the just in time scheduler
        - yml_parameter: the application parameter management program


There's several kind of queries available in YML. We will focus on the
two following :
        - Component generation
        - Application execution
When working with YML you will always follow this simple workflow:
    - Create abstract components
    - Create implementation components
    - Create application
    - Run application

Let's start with the study of the definition of an abstract
component. The query bellow stored in a file named abstract/test.query
contains the definition of the only component we will use.


	<?xml version="1.0"?>
	<component type="abstract" name="test"
        	description="This component sums two double and stores the result in res">
    	<params>
        	<param name="res" type="real" mode="out" />
        	<param name="a1"  type="real" mode="in" />
        	<param name="a2"  type="real" mode="in" />
    	</params>
	</component>

To register an abstract component, one have to use
yml_component. using the following command (TODO):

$> yml_component abstract/test.query

####TODO

- Complete the .xml file in abstract/
- Register the abstract component

The file impl/test_impl.query contains the following :

	<?xml version="1.0" ?>
	<component name="test_impl" abstract="test" type="impl">
	    <impl lang="CXX" libs="">
	        <header></header>
	        <source>
	            res = a1 + a2;
	        </source>
	        <footer></footer>
	    </impl>
	</component>

An implementation component is used to describe the computation
executed remotely. It's compiled in a static binary application
integrating communication mechanism required by YML.

Once again we can build this component using the component generator
$> yml_component impl/test_impl.query

if you already register a component and you want to overwrite
your previously registered component just add the --force option in that
way:
$> yml_component --force impl/test_impl.query


####TODO

- Complete the .xml file in impl/

- Build the component with the component generator


####TODO

- List files which are in $PREFIX/var/yml/DefaultDevelopmentCatalog/

- Open and read .xml files.


Finally the latest query we need is the application query which
describe the graph to be executed. In this example we will make two
sequential sum. The application is described in the file
apps/test1.query.

	<application name="test1">
	    <description>A starter example of an YML application</description>
	    <params>
	    </params>
	    <graph>
	        compute test(result, 1.0, 2.0);       # result contains 3.0
	        compute test(result, result, result); # result contains 6.0
	    </graph>
	</application>

one can compile this application using

$> yml_compiler test1.query

A file test1.query.yapp is generated.


####TODO

- Compile this application.

- List files which are in $PREFIX/var/yml/dr/binaries

- Open and read .cc files.

This operation generate a file test1.query.yapp containing an
application ready to be executed by the scheduler:

$> yml_scheduler test1.query.yapp


####TODO

- Execute this application.

- Read and note the information about the execution (such as
the number of tasks).

This command will create a folder named runXXX where XXX represent an
integer. this folder contains all information related to an execution
of an application. This include the graph, and some other information.


####TODO

- In particular you can look at the content of the runXXX/exec_log
which provide a list of the component call or task executed on the
underlying middleware.

- This command also create a runXXX_results.pack
file which contains the output parameter of the application. This file
can be inspected using the yml_parameter tool. However this does not
give any result for now on because the application does not define any
parameters.

In order to illustrate this tool we will update our application
definition a little in order to add parameters.
	<application name="test2">
	    <description>
	A starter example of an YML application with parameter
	    </description>
	    <params>
	        <param name="v1" type="real" mode="in" />
	        <param name="v2" type="real" mode="in" />
	        <param name="result" type="real" mode="out" />
	    </params>
	    <graph>
	        compute test(result, v1, v2);
	        compute test(result, result, result);
	    </grap>
	</application>

This application now takes 2 input parameters v1 and v2 both reals. It
create a real as a result called result. Let's compile this application
using the --output parameter to be able to specify the name of the
resulting file:

$> yml_compiler --output=test2.yapp test2.query


####TODO

- Add the parameters in the test2.query file.

- Compile it.

####TODO

Now follow the tutorial on how to manipulate parameters with

yml_parameter.

We can display parameter related application using yml_parameter tool

$> yml_parameter --list --app=test2.yapp

If you want to list only inputs(outputs) you can do this using the
--list-input flag (--list-output)

$> yml_parameter --list-input --app=test2.yapp

It might be interesting to check the status of a pack while dealing with
them. This can be done using the --status (--status-input, --status-output)
flag. This command gives you a summary of file provided in the pack and file
not yet available.

$> yml_parameter --status --app=test2.yapp --pack=pack_test2.pack

Let's now present the yml_parameter tool main goal. It is used to manipulate
pack files given in inputs and outputs of an application. We will first focus
on input parameters. In the application above we need two real parameter. We can
specify them using :

$> yml_parameter --app=test2.yapp --pack=test2_input.pack --add=v1 --real=1.0
$> yml_parameter --app=test2.yapp --pack=test2_input.pack --add=v2 --real=3.0

If you use several time the --add command on the same variable only the last
one will be taken into account. You can check the value of a parameter using :

$> yml_parameter --app=test2.yapp --pack=test2_input.pack --extract=v2

This command will output the value of v2 to the output of the yml_parameter tool.

The command below will execute the application using test2_input.pack as an input pack.

$> yml_scheduler --input=test2_input.pack test2.yapp

You can see the value of result using :

$> yml_parameter --app=test2.yapp --pack=runXXX_results.pack --extract=result


## TUTORIAL 2: Distributed sort algorithm       

Follow the tutorial and the TODO points.

It is assumed that you have already followed tutorial 1 (../tutorial1-sum)

This is a nice example of parallelism express using YML to represent a set
of parallel loop with explicit synchronisation. It is extracted from the PhD
 Thesis of Olivier Delannoy (p.88)

1./ Overview of the algorithm

This application presents a distributed sorting of a large dataset. The
dataset is composed of integers stored in blocks of a fixed size. The
application relies mostly on an operation we called merge. This operation
takes two blocks of a_i and a_j and creates two new blocks a_i and a_j of the
same sizes. The elements of block ai are all smaller than elements of block
a_j. All outputs blocks are also sorted. The sort algorithm is described by
algorithm 1.

###Algorithm 1

INPUTS: {a_1 , a_2 , . . . , a_n } N unsorted blocks of K integer values.
OUTPUTS: {a_1 , a_2 , . . . , a_n } N sorted blocks of K integer values.

for i in 1, blockCount in parallel do
    DSortBlock (INPUTS: ai OUTPUTS: ai )
for iter in 1, blockCount − 1 do
    inc ← 1 + iter mod 2
    for i in 0, blockCount/2 − 1 in parallel do
        block1 ← 1 + (inc + 2 ∗ i) mod N
        block2 ← 1 + (inc + 2 ∗ i + 1) mod N
        min ← min(block1, block2)
        max ← max(block1, block2)
        DSortMerge (INPUTS: a_min , a_max OUTPUTS: a_min , a_max )

2./ Conception of the application

Parameters and services needed for the application are listed.

It defines two parameters:

- blockSize: defines the number of integers contained in each block.

- blockCount: defines the number of blocks of blockSize integers. The size of
the collection to sort is obtained by multiplying blockSize by blockCount.
This parameter also determines the number of parallel operations which compose
its iteration of the algorithm.

The 3 services are:
- Generator: create a block of the collection composed of a random integers.

- Sort: Sort the elements contained in a single block. This is only needed
after the generation of a block because the merging operation expects input
blocks to be sorted.

- Merge: Merge two blocks as described before. The two blocks are merged so that
the smallest elements are in the rst resulting block and the largest elements t in
the second resulting block. The number remains sorted in both blocks

Workflow:
Figure 1 is a visual representation of the work of the application using
blockCount = 4 blocks.


###Figure 1 - Workflow of DSort application for blockCount = 4 blocks (Top -> Down)

                        -------------------------
                        |        <Start>        |
                        -------------------------
                        /       |       |       \
                       /        |       |        \
                  -------    -------  -------  -------
                  | Gen |    | Gen |  | Gen |  | Gen |
                  -------    -------  -------  -------
                     |          |       |         |
                     |          |       |         |
                  -------    -------  -------  -------
                  | Sort|    | Sort|  | Sort|  | Sort|
                  -------    -------  -------  -------
                      \         /        \        /
                       \       /          \      /
                        -------            -------
                        |Merge|            |Merge|
                        -------            -------
                         |    |   __________|   |
                         |    |___|_________    |
                         |     ___|         |   |
                         |    |             |   |
                        -------            -------
                        |Merge|            |Merge|
                        -------            -------
                         |    |   __________|   |
                         |    |___|_________    |
                         |     ___|         |   |
                         |    |             |   |
                        -------            -------
                        |Merge|            |Merge|
                        -------            -------
                               \          /
                                \        /
                                 \      /
                                 --------
                                 |<Stop>|
                                 --------

3./ Implementation in YML

3.1./ Abstract components

- DSortGenerator.query

	<?xml version="1.0"?>
	<component type="abstract" name="DSortGenerator" description="Create a block of random integer" >
		<params>
		    <param name="block" type="VectorInteger" mode="out" />
		    <param name="size"  type="integer" mode="in" />
		    <param name="max"   type="real" mode="in" />
		</params>
	</component>

- DSortSort.query

	<?xml version="1.0" ?>
	<component name="DSortSort" type="abstract" description="Sort a single block">
	    <params>
	        <param name="block" type="VectorInteger" mode="inout" />
	    </params>
	</component>


- DSortMerge.query

	<?xml version="1.0" ?>
	<component name="DSortSort" type="abstract" description="Sort a single block">
	    <params>
	        <param name="block" type="VectorInteger" mode="inout" />
	    </params>
	</component>


- DSortMerge.query

	<?xml version="1.0"?>
	<component name="DSortMerge" type="abstract" description="Sort two sorted block
	and store all values in two block all value in b1Out are lower or equal to
	the ones in b2Out" >
	    <params>
	        <param name="b1Out" type="VectorInteger" mode="out" />
	        <param name="b2Out" type="VectorInteger" mode="out" />
	        <param name="b1In" type="VectorInteger" mode="in" />
	        <param name="b2In" type="VectorInteger" mode="in" />
	    </params>
	</component>

3.2./ Implementation components

See implementation of each component in ./impl

- DSortGenerator_impl.query

- DSortSort_impl.query

- DSortMerge_impl.query

3.3./ Graph

####TODO

- Complete the graph of this application in ./application/execution.query
considering algorithm 1 and Figure 1.

- execution.query

- Implementation of Algorithm 1 (Complete the file execution.query)

4./ Compilation and execution

Remark:
Here we use the type vector for integers. YML need a definition of this type.
This is done writing a .type.hh file following the instruction in cxx_include.README
cxx_include.README and cxx types for YML are located in the install directory:
$PREFIX/var/yml/DefaultExecutionCatalog/generators/CXX/include/

Here, the .type.hh file is written in ./types.

Then, ./types/*.type.hh should be copyied in $PREFIX/var/yml/DefaultExecutionCatalog/generators/CXX/include/
Read cxx_include.README for information about type definition.

####TODO

- Register components, and compile the application using
the command yml_component, yml_compiler, yml_parameter
(to pass blockSize), yml_scheduler
as described in tutorial1-sum.