/**
 * 
 */
package algorithms;

import java.io.FileInputStream;
import java.util.Collections;
import java.util.LinkedList;
import java.util.Properties;

import util.*;
import xml.XMLParser;

/**
 * @author riesen
 * 
 */
public class GraphMatching {

	//GraphSet:LinkedList<Graph>(双向链表实现）
	private GraphSet source, target;

	/**
	 * the resulting distance matrix D = (d_i,j), where d_i,j = d(g_i,g_j) 
	 * (distances between all graphs g_i from source and all graphs g_j from target) 
	 */
	private double[][] distanceMatrix;
	//	private double[][] assignmentCostMatrix;

	/**
	 * the source and target graph actually to be matched (temp ist for temporarily swappings)
	 */
	private Graph sourceGraph, targetGraph, temp;

	/**
	 * whether the edges of the graphs are undirected (=1) or directed (=0)
	 */
	private int undirected;

	/**
	 * progess-counter
	 */
	private int counter;

	/**
	 * log options:
	 * output the individual graphs
	 * output the cost matrix for bipartite graph matching
	 * output the matching between the nodes based on the cost matrix (considering the local substructures only)
	 * output the edit path between the graphs
	 */
	private int outputGraphs;
	private int outputCostMatrix;
	private int outputMatching;
	private int outputEditpath;

	/**
	 * the cost function to be applied
	 */
	private CostFunction costFunction;//计算Node与Node，edge与edge之间的代价

	/**
	 * number of rows and columns in the distance matrix
	 * (i.e. number of source and target graphs)
	    defalut contindition: r > c (swap)
	 */
	private int r;
	private int c;

	/**
	 * computes an optimal bipartite matching of local graph structures
	 */
	private BipartiteMatching bipartiteMatching;

	/**
	 * computes the approximated or exact graph edit distance 
	 */
	private EditDistance editDistance;

	/**
	 * the matching procedure defined via GUI or properties file
	 * possible choices are 'Hungarian', 'VJ' (VolgenantJonker)
	 * 'AStar' (exact tree search) or 'Beam' (approximation based on tree-search)
	 */
	private String matching;

	/**
	 * the maximum number of open paths (used for beam-search)
	 */
	private int s;
	/**
	 * generates the cost matrix where on the optimal bipartite matching can
	 * be computed
	 */
	private MatrixGenerator matrixGenerator;
	/**
	 * whether or not a similarity kernel is built upon the distance values:
	 * 0 = distance matrix is generated: D = (d_i,j), where d_i,j = d(g_i,g_j) 
	 * 1 = -(d_i,j)^2
	 * 2 = -d_i,j
	 * 3 = tanh(-d)
	 * 4 = exp(-d)
	 */
	private int simKernel;
	/**
	 * prints the results
	 */
	private ResultPrinter resultPrinter;
	private String adj; // best or worst or none
	/**
	 * @param properties 
	 * properties[0] is an url to a properties file, where all parameters are defined
	 * (e.g. /Users/riesen/Documents/GraphMatching/properties/testproperties.prop)
	 */
	@SuppressWarnings("unused")
	public static void main(String[] properties)
	{
		try 
		{
			GraphMatching graphMatching = new GraphMatching("./properties/properties_letter.prop");
		} 
		catch (Exception e) {
			e.printStackTrace();
		}
		
	}
	/**
	 * the matching procedure
	 * @throws Exception
	 */
	public GraphMatching(String prop) throws Exception 
	{
		// initialize the matching
		System.out.println("Initializing the matching according to the properties...");
		this.init(prop);//得到配置文件
		//init the graph data 
		// the cost matrix used for bipartite matchings
		double[][] costMatrix;

		//counts the progress
		this.counter = 0;
		//iterate through all pairs of graphs g_i x g_j from (source, target)
		System.out.println("Starting the matching...");
		System.out.println("Progress...");
		int numOfMatchings = this.source.size() * this.target.size();
		int numOfFails = 0;
		// distance value d
		double d = -1;

		// swapped the graphs?
		boolean swapped = false;
		long startTime = System.currentTimeMillis();
		for (int i = 0; i < r; i++) 
		{
			sourceGraph = this.source.get(i);
			for (int j = 0; j < c; j++)
			{
				swapped = false;
				targetGraph = this.target.get(j);
				this.counter++;
				System.out.println("Matching "+counter+" of "+numOfMatchings+" (sizes: "+sourceGraph.size()+", "+targetGraph.size()+")");

				// log the current graphs on the console
				if (this.outputGraphs == 1) {
					System.out.println("The Source Graph:");
					System.out.println(sourceGraph);
					System.out.println("\n\nThe Target Graph:");
					System.out.println(targetGraph);
				}
				// if both graphs are empty the distance is zero and no computations have to be carried out!
				if (this.sourceGraph.size() < 1 && this.targetGraph.size() < 1)
				{
					d = 0;
				} 
				else {
					// calculate the approximated or exact edit-distance using tree search algorithms 
					// AStar: number of open paths during search is unlimited (s=infty)
					// Beam: number of open paths during search is limited to s
					if (this.matching.equals("AStar") || this.matching.equals("Beam"))
					{
						//beam search: 
						d = this.editDistance.getEditDistance(
								sourceGraph, targetGraph, costFunction, 10000000, 1000);
						if (d == -1.00){
							System.out.println("Fail Nr. "+numOfFails++);
						}
						System.out.println(d);
					} 
					else
					{ 
						// approximation of graph edit distances via bipartite matching
						// in order to get determinant edit costs between two graphs
						if (this.sourceGraph.size() < this.targetGraph.size())
						{
							this.swapGraphs();
							swapped= true;
						}
						// compute the matching using Hungarian or VolgenantJonker (defined in String matching)
						int[][] matching = null;
						// generate the cost-matrix between the local substructures of the source and target graphs
						costMatrix = this.matrixGenerator.getMatrix(sourceGraph, targetGraph);
						matching = this.bipartiteMatching.getMatching(costMatrix);
						d = this.editDistance.getEditDistance(sourceGraph,
								targetGraph, matching, costFunction);
					}
				}
				// whether distances or similarities are computed
				if (this.simKernel < 1)
				{
					this.distanceMatrix[i][j] = d;
					//					this.assignmentCostMatrix[i][j] = assignmentCost;
				} else {
					switch (this.simKernel){
					case 1:
						this.distanceMatrix[i][j] = -Math.pow(d,2.0);break;
					case 2:
						this.distanceMatrix[i][j] = -d;break;
					case 3:
						this.distanceMatrix[i][j] = Math.tanh(-d);break;
					case 4:
						this.distanceMatrix[i][j] = Math.exp(-d);break;
					}			
				}
				if (swapped){
					this.swapGraphs();
				}
			}
		}

		long endingTime = System.currentTimeMillis();
		long matchingTime = endingTime - startTime;
		// printing the distances or similarities
		System.out.println("Printing the results...");
			//		this.resultPrinter.printResult(this.distanceMatrix, this.assignmentCostMatrix, matchingTime, numOfFails);
	}




	/**
	 * swap the source and target graph
	 */
	private void swapGraphs() {
		this.temp = this.sourceGraph;
		this.sourceGraph = this.targetGraph;
		this.targetGraph = this.temp;
	}



	/**
	 * initializes the whole graph edit distance framework according to the properties files 
	 * @param prop
	 * @throws Exception
	 */
	
	private void init(String prop) throws Exception 
	{
		// load the properties file
		Properties properties = new Properties();
		properties.load(new FileInputStream(prop));

		// define result folder
		String resultFolder = properties.getProperty("result");

		// the node and edge costs, the relative weighting factor alpha
		double node = Double.parseDouble(properties.getProperty("node"));
		double edge = Double.parseDouble(properties.getProperty("edge"));
		double alpha = Double.parseDouble(properties.getProperty("alpha"));

		// the node and edge attributes (the names, the individual cost functions, the weighting factors)
		int numOfNodeAttr = Integer.parseInt(properties
				.getProperty("numOfNodeAttr"));
		int numOfEdgeAttr = Integer.parseInt(properties
				.getProperty("numOfEdgeAttr"));
		String[] nodeAttributes = new String[numOfNodeAttr];
		String[] edgeAttributes = new String[numOfEdgeAttr];
		String[] nodeCostTypes = new String[numOfNodeAttr];
		String[] edgeCostTypes = new String[numOfEdgeAttr];
		double[] edgeCostMu = new double[numOfEdgeAttr];
		double[] edgeCostNu = new double[numOfEdgeAttr];
		double[] nodeAttrImportance = new double[numOfNodeAttr];
		double[] edgeAttrImportance = new double[numOfEdgeAttr];
		double[] nodeCostMu = new double[numOfNodeAttr];
		double[] nodeCostNu = new double[numOfNodeAttr];
		for (int i = 0; i < numOfNodeAttr; i++) {
			nodeAttributes[i] = properties.getProperty("nodeAttr" + i);
			nodeCostTypes[i] = properties.getProperty("nodeCostType" + i);
			if (nodeCostTypes[i].equals("discrete"))
			{
				nodeCostMu[i]=Double.parseDouble(properties.getProperty("nodeCostMu" + i));
				nodeCostNu[i]=Double.parseDouble(properties.getProperty("nodeCostNu" + i));
			}
			if  (!nodeCostTypes[i].equals("coil"))
			{
				nodeAttrImportance[i] = Double.parseDouble(properties
						.getProperty("nodeAttr" + i + "Importance"));
			}

		}
		for (int i = 0; i < numOfEdgeAttr; i++) 
		{
			edgeAttributes[i] = properties.getProperty("edgeAttr" + i);
			edgeCostTypes[i] = properties.getProperty("edgeCostType" + i);
			edgeAttrImportance[i] = Double.parseDouble(properties
					.getProperty("edgeAttr" + i + "Importance"));
			if (edgeCostTypes[i].equals("discrete"))
			{
				edgeCostMu[i]=Double.parseDouble(properties.getProperty("edgeCostMu" + i));
				edgeCostNu[i]=Double.parseDouble(properties.getProperty("edgeCostNu" + i));
			}
		}

		// whether or not the costs are "p-rooted"
		double squareRootNodeCosts = Double.parseDouble(properties
				.getProperty("pNode"));
		double squareRootEdgeCosts = Double.parseDouble(properties
				.getProperty("pEdge"));

		// whether costs are multiplied or summed
		int multiplyNodeCosts = Integer.parseInt(properties
				.getProperty("multiplyNodeCosts"));
		int multiplyEdgeCosts = Integer.parseInt(properties
				.getProperty("multiplyEdgeCosts"));

		// what is logged on the console (graphs, cost-matrix, matching, edit path)
		this.outputGraphs = Integer.parseInt(properties
				.getProperty("outputGraphs"));
		this.outputCostMatrix = Integer.parseInt(properties
				.getProperty("outputCostMatrix"));
		this.outputMatching = Integer.parseInt(properties
				.getProperty("outputMatching"));
		this.outputEditpath = Integer.parseInt(properties
				.getProperty("outputEditpath"));
		// whether the edges of the graphs are directed or undirected
		this.undirected = Integer
				.parseInt(properties.getProperty("undirected"));

		// the graph matching paradigm actually employed 		
		this.matching =  properties.getProperty("matching");

		// maximum number of open paths is limited to s in beam-search
		if (this.matching.equals("Beam") || this.matching.equals("reverseGED")){
			this.s =  Integer.parseInt(properties.getProperty("s"));
		} 
		else {
			this.s = Integer.MAX_VALUE; // AStar，精确计算两个图之间的编辑距离
		}
		this.adj = properties.getProperty("adj");
		// initialize the cost function according to properties
		this.costFunction = new CostFunction(node, edge, alpha, nodeAttributes,
				nodeCostTypes, nodeAttrImportance, edgeAttributes,
				edgeCostTypes, edgeAttrImportance, squareRootNodeCosts,
				multiplyNodeCosts, squareRootEdgeCosts, multiplyEdgeCosts, nodeCostMu, nodeCostNu,
				edgeCostMu, edgeCostNu);	


		// the matrixGenerator generates the cost-matrices according to the costfunction
		this.matrixGenerator = new MatrixGenerator(this.costFunction,
				this.outputCostMatrix);
		this.matrixGenerator.setAdj(this.adj);

		// bipartite matching procedure (Hungarian or VolgenantJonker)
		this.bipartiteMatching = new BipartiteMatching(this.matching, this.outputMatching);

	

		// editDistance computes either the approximated edit-distance according to the bipartite matching 
		// or computes the exact edit distance
		this.editDistance = new EditDistance(this.undirected, this.outputEditpath);

		// the resultPrinter prints the properties and the distances found		
		this.resultPrinter = new ResultPrinter(resultFolder, properties);

		// whether or not a similarity is derived from the distances 
		this.simKernel=Integer.parseInt(properties.getProperty("simKernel"));

		// load the source and target set of graphs
		System.out.println("Load the source and target graph sets...");
		XMLParser xmlParser = new XMLParser();
		xmlParser.setGraphPath(properties.getProperty("path"));
		String sourceString = properties.getProperty("source");
		String targetString = properties.getProperty("target");
		this.source = xmlParser.parseCXL(sourceString);
		//System.out.println("A success!!!");
		this.target = xmlParser.parseCXL(targetString);

		// create a distance matrix to store the resulting dissimilarities
		this.r = this.source.size();
		this.c = this.target.size();
		this.distanceMatrix = new double[this.r][this.c];
		//		this.assignmentCostMatrix = new double[this.r][this.c];

	}
}
	/**
	 * @return the progress of the matching procedure
	 */
	