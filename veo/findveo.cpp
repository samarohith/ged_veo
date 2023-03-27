#include "veo.h"
#include <set>
#include <iterator>
#include<string>
#include<bits/stdc++.h>


using namespace std;

// For parsing the input graph dataset
void parseGraphDataset(ifstream &inp, vector<Graph> &graph_dataset, int &dataset_size);

// Sorts vertex and edge set of each graph in the dataset
void sortGraphDataset(vector<Graph> &graph_dataset);

// Graph comparator
bool graphComp(Graph &g1, Graph &g2);

// Returns the time from start to end in milliseconds
unsigned long long int clocksTosec(chrono::high_resolution_clock::time_point start, chrono::high_resolution_clock::time_point end);

// Displays the memory used by the program(in MB)
double memoryUsage();

// prints correct usage of program in case of an error
void usage();

// loose:   ./filter inp_file 1 simScore_threshold dataset-size res-file
// strict:  ./filter inp_file 2 simScore_threshold mismatch dataset-size res-file
//		     		  false/true : 0/1
// static:  ./filter inp_file 3 simScore_threshold mismatch noofbuckets dataset-size res-file
// dynamic: ./filter inp_file 4 simScore_threshold mismatch noofbuckets dataset-size res-file

//Taking as ordered_set. set bcoz duplicates are not allowed. ordered bcoz I need in Increasing order.
set<char> global_vrtxlbl_set;
unordered_map<char, unsigned> global_vrtxlbl_map;  //to map global vertex label to unsigned numeric.
unordered_map<string, unsigned> global_edgetype_map;


int main(int argc, char const *argv[])
{

	vector<Graph> graph_dataset; // input graph dataset
	string name = argv[1];
	ifstream dataset_file(name + "/" + name+"_veo.txt");
	ifstream fin(name + "/" + name+"_veo.txt");
  ifstream ged_file(name + "/" + "result.txt");
	if(!dataset_file.is_open())
	{
		cerr << "Unable to open dataset file" << endl;
		exit(0);
	}
	if(!ged_file.is_open())
	{
		cerr << "Unable to open ged file" << endl;
		exit(0);
	}
	int dataset_size = 0;
	string line;

	while(fin)
	{
		getline(fin,line);
		dataset_size = stoi(line);
		break;
	}
	cout<<dataset_size<<endl;
	fin.close();

	// parsing input graph-dataset

	parseGraphDataset(dataset_file, graph_dataset, dataset_size);
	cout << "Graph Dataset parsed.\n";

	// Sorts vertex and edge set of each graph in the dataset
	//sortGraphDataset(graph_dataset);
	// sorts the graph dataset
	//sort(graph_dataset.begin(), graph_dataset.end(), graphComp);

	double simScore; // similarity score
	vector<int> array_info(6,200);
	double common = 0;

	VEO veo_sim = VEO(50);
  //VEO veo_sim;

 	// timestamping start time
	chrono::high_resolution_clock::time_point cl0 = chrono::high_resolution_clock::now();
  line;
  ofstream fout(name + "/array_info.txt");

  while(ged_file)
  {
      getline(ged_file, line);
      if(line == "") break;
      vector <string> tokens;
      stringstream ss(line);
      string temp;
			common = 0;
      while(getline(ss, temp, ' '))
      {
          tokens.push_back(temp);
      }
      int g1 = stoi(tokens[0])-1, g2 = stoi(tokens[1])-1, ged = stoi(tokens[2]);
      Graph G1 = graph_dataset[g1], G2 = graph_dataset[g2];
      if(G1.vertexCount < 20 || G2.vertexCount < 20) continue;
    	simScore = veo_sim.computeSimilarity(graph_dataset[g1], graph_dataset[g2], common);
    	if(array_info[ged] > simScore) 
    	{
    		array_info[ged] = simScore;
    		if(ged == 4) cout<<"graph ids are "<<g1<<" "<<g2<<" "<<simScore<<endl;
    	}
  }

  for(int i = 0; i < 6; i++)
  	fout<<i<<" "<<array_info[i]<<endl;

  ged_file.close();
  fout.close();

	chrono::high_resolution_clock::time_point cl2 = chrono::high_resolution_clock::now();
	int totalTimeTaken = (clocksTosec(cl0,cl2));
  cout<<"total time taken = "<<totalTimeTaken<<endl;
	return 0;
}

// parses the input graph dataset and query graph
void parseGraphDataset(ifstream &inp, vector<Graph> &graph_dataset, int &dataset_size)
{
	int i = 0,size, vlblCount;
  unsigned vlbl_unsigned =0, edge_type_unsign =0;
	inp >> size;
	//inp >>vlblCount;
	if(dataset_size == -1)
		dataset_size= size;

	  for(int i = 0; i <= 100; i++)
		{
	       for(int j=i; j<=100; j++)
	       {
					 string str1 =to_string(i), str2 =to_string(j);
					 string edge_type_str =str1;
					 edge_type_str += '-';
					 edge_type_str += str2;
	         global_edgetype_map[edge_type_str] = edge_type_unsign++;
	       }

	   }


	graph_dataset.resize(dataset_size);

	for(auto g_iter = graph_dataset.begin(); g_iter != graph_dataset.end(); g_iter++)
	{
		g_iter->readGraph(inp, vlblCount,  global_vrtxlbl_map, global_edgetype_map);
	}
}

bool graphComp(Graph &g1, Graph &g2)
{
	return g1.vertexCount+g1.edgeCount < g2.vertexCount+g2.edgeCount;
}

// Sorts vertex and edge set of graph dataset
void sortGraphDataset(vector<Graph> &graph_dataset)
{
	for(int i = 0; i < graph_dataset.size(); i++)
	{
		sort(graph_dataset[i].vertices.begin(), graph_dataset[i].vertices.end()); // sort vertex-set
		sort(graph_dataset[i].edges.begin(), graph_dataset[i].edges.end()); // sort edge-set
	}
}

// Returns the time from start to end in seconds
unsigned long long int clocksTosec(chrono::high_resolution_clock::time_point start, chrono::high_resolution_clock::time_point end)
{
	return (unsigned long long int)(1e-6*chrono::duration_cast<chrono::nanoseconds>(end - start).count());
}

// Displays the memory used by the program(in MB)
double memoryUsage()
{
	struct rusage r_usage;
	getrusage(RUSAGE_SELF, &r_usage);
	return r_usage.ru_maxrss/1024.0;
}

// prints correct usage of program in case of an error
void usage(){
	cerr << "usage: ./filter input_file choice simScore-threshold dataset-size res-dir" <<endl;

	cerr << "Available choices: " << endl;
	cerr << endl;

	cerr << "1 loose : 1" << endl;
	cerr << endl;

	cerr << "2 loose + strict 			  : 2, mismatch=false noofbuckets=0" << endl;
	cerr << "3 loose + strict + mismatch  : 2, mismatch=true  noofbuckets=0" << endl;
	cerr << endl;

	cerr << "4 loose + strict + static 				: 3, mismatch=false noofbuckets = 0" << endl;
	cerr << "5 loose + strict + static + 2 buckets  : 3, mismatch=false noofbuckets = 2" << endl;
	cerr << "6 loose + strict + static + 5 buckets  : 3, mismatch=false noofbuckets = 5" << endl;
	cerr << "7 loose + strict + static + 10 buckets : 3, mismatch=false noofbuckets = 10" << endl;
	cerr << "8 loose + strict + static + 10 buckets : 3, mismatch=true  noofbuckets = 10" << endl;
	cerr << "9  loose + strict + dynamic 			  :  4, mismatch=false noofbuckets = 0" << endl;
	cerr << "10 loose + strict + dynamic + 2 buckets  :  4, mismatch=false noofbuckets = 2" << endl;
	cerr << "11 loose + strict + dynamic + 5 buckets  :  4, mismatch=false noofbuckets = 5" << endl;
	cerr << "12 loose + strict + dynamic + 10 buckets :  4, mismatch=false noofbuckets = 10" << endl;
	cerr << "13 loose + strict + dynamic + 10 buckets :  4, mismatch=true  noofbuckets = 10" << endl;
	cerr << endl;

	cerr << "loose:   ./filter inp_file 1 simScore_threshold dataset-size res-dir\n";
	cerr << "strict:  ./filter inp_file 2 simScore_threshold mismatch dataset-size res-dir\n";
//		     											  false/true : 0/1
	cerr << "static:  ./filter inp_file 3 simScore_threshold mismatch noofbuckets dataset-size res-dir\n";
	cerr << "dynamic: ./filter inp_file 4 simScore_threshold mismatch noofbuckets dataset-size res-dir\n";
	exit(0);
}
