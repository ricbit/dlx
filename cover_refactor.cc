#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <set>
#include <fstream>

using namespace std;

struct Header {
  int llink;
  int rlink;
};

struct Node {
  int top;
  int ulink;
  int dlink;
};

class Graph {
 public:
  Graph(const vector<Header>& header, const vector<Node>& nodes, const vector<string>& names)
      : last_header(begin(header), end(header)),
        last_nodes(begin(nodes), end(nodes)),
        names(names), frame(0) {
  }

  void graph(const vector<Header>& header, const vector<Node>& nodes) {
    ostringstream oss;
    //cout << "creating frame " << frame << "\n";
    oss << "frame" << frame++ << ".dot";
    ofstream ofs;
    ofs.open(oss.str());
    ofs << "digraph dancing {\n";
    ofs << "graph [ splines=\"ortho\" overlap=\"scalexy\" ";
    ofs << "sep=\"2\" pad=\"1,0.25\" esep=\"1\" ];\n";
    int option = 0;
    for (int i = 0; i < int(nodes.size()); i++) {
      ofs << "n" << i << " [label=\"";
      if (i >= 1 && i < int(header.size())) {
        ofs << names[i] << " ";
      } else if (i >= int(header.size()) && nodes[i].top > 0) {
        ofs << names[nodes[i].top] << " ";
      }
      ofs << "(" << i << ")\" ";
      ofs << "shape=\"box\" ";
      if (i < int(header.size())) {
        ofs << "pos=\"" << i << "," << "0!\" ";
      } else if (nodes[i].top <= 0) {
        ofs << "pos=\"0,-" << ++option << "!\" ";
      } else {
        ofs << "pos=\"" << nodes[i].top << ",-" << option << "!\" ";
      }
      ofs << "] ;\n";
    }
    for (int i = 0; i < int(header.size()); i++) {
      link(ofs, i, header[i].rlink, last_header[i].rlink, "dashed");
      link(ofs, i, header[i].llink, last_header[i].llink, "dashed");
    }
    for (int i = 0; i < int(nodes.size()); i++) {
      link(ofs, i, nodes[i].dlink, last_nodes[i].dlink, "solid");
      link(ofs, i, nodes[i].ulink, last_nodes[i].ulink, "solid");
    }
    ofs << "}";
    ofs.close();
    copy(begin(header), end(header), begin(last_header));
    copy(begin(nodes), end(nodes), begin(last_nodes));
  }
 private:
  void link(ofstream& ofs, int a, int b, int last_b, string style) {
    if (b < 0) {
      return;
    }
    ofs << "n" << a << " -> n" << b << " [ style=\"" << style << "\" ";
    if (b != last_b) {
      ofs << "color=\"red\" ";
    }
    ofs << "] ;\n";
  }
  vector<Header> last_header;
  vector<Node> last_nodes;
  const vector<string>& names;
  int frame;
};

class ExactCover {
 public:
  ExactCover(
      const vector<string>& items,
      const vector<vector<string>>& options)
      : header(build_header(items)), nodes(build_nodes(items, options)),
        names(items), solution(options.size()), graph(header, nodes, items) {
  }

  void print() {
    cout << "nodes " << nodes.size() << "\n";
    auto print_line = [&](int j, int size, auto func) {
      for (int i = j * 8; i < j * 8 + 8 && i < size; i++) {
        cout << func(i) << "\t";
      }
      cout << "\n";
    };
    for (int j = 0; j <= int(header.size()) / 8; j++) {
      print_line(j, header.size(), [&](int i) { return i; });
      print_line(j, header.size(), [&](int i) { return i ? names[i - 1] : ""; });
      print_line(j, header.size(), [&](int i) { return header[i].llink; });
      print_line(j, header.size(), [&](int i) { return header[i].rlink; });
    }
    for (int j = 0; j <= int(nodes.size()) / 8; j++) {
      print_line(j, nodes.size(), [&](int i) { return i; });
      print_line(j, nodes.size(), [&](int i) { return nodes[i].top; });
      print_line(j, nodes.size(), [&](int i) { return nodes[i].ulink; });
      print_line(j, nodes.size(), [&](int i) { return nodes[i].dlink; });
      cout << "\n";
    }
  }

  void write_dot() {
    graph.graph(header, nodes);
  }

  long run() {
    count = 0;
    solve(0);
    return count;
  }

 private:
  void solve(int level) {
    if (header[0].rlink == 0) {
      count++;
      return;
    }
    int item = best_item();
    cover(item);
    solution[level] = nodes[item].dlink;
    for (auto& option = solution[level]; option != item; option = nodes[option].dlink) {
      try_option(option);
      solve(level + 1);
      rewind_option(option);
    }
    uncover(item);
  }

  int best_item() {
    int best = header[0].rlink;
    int value = nodes[best].top;
    for (int p = header[best].rlink; p != 0; p = header[p].rlink) {
      if (nodes[p].top < value) {
        best = p;
        value = nodes[p].top;
        if (value == 0) {
          break;
        }
      }
    }
    return best;
  }

  template<typename T>
  void option_right(int option, T action) {
    for (auto p = option + 1; p != option;) {
      if (nodes[p].top <= 0) {
        p = nodes[p].ulink;
      } else {
        action(p);
        p++;
      }
    }
  }

  template<typename T>
  void option_left(int option, T action) {
    for (auto p = option - 1; p != option;) {
      if (nodes[p].top <= 0) {
        p = nodes[p].dlink;
      } else {
        action(p);
        p--;
      }
    }
  }

  void try_option(int option) {
    option_right(option, [&](int p) {
      cover(nodes[p].top);
    });
  }

  void rewind_option(int option) {
    option_left(option, [&](int p) {
      uncover(nodes[p].top);
    });
  }

  void cover(int i) {
    for (auto p = nodes[i].dlink; p != i; p = nodes[p].dlink) {
      hide(p);
    }
    auto l = header[i].llink;
    auto r = header[i].rlink;
    header[l].rlink = r;
    header[r].llink = l;
  }

  void uncover(int i) {
    auto l = header[i].llink;
    auto r = header[i].rlink;
    header[l].rlink = i;
    header[r].llink = i;
    for (auto p = nodes[i].ulink; p != i; p = nodes[p].ulink) {
      unhide(p);
    }
  }

  void hide(int p) {
    option_right(p, [&](int q) {
      auto x = nodes[q].top;
      auto u = nodes[q].ulink;
      auto d = nodes[q].dlink;
      nodes[u].dlink = d;
      nodes[d].ulink = u;
      nodes[x].top--;
    });
  }

  void unhide(int p) {
    option_left(p, [&](int q) {
      auto x = nodes[q].top;
      auto u = nodes[q].ulink;
      auto d = nodes[q].dlink;
      nodes[u].dlink = q;
      nodes[d].ulink = q;
      nodes[x].top++;
    });
  }

  void dump_current() {
    cout << "items remaining: ";
    for (int t = 0; header[t].rlink != 0;) {
      t = header[t].rlink;
      cout << names[t] << " ";
    }
    cout << "\n";
    cout << "options remaining:\n";
    set<int> options;
    for (int p = header[0].rlink; p != 0; p = header[p].rlink) {
      for (int i = nodes[p].dlink; i != p; i = nodes[i].dlink) {
        int j = i;
        for (; nodes[j].top > 0; j--);
        options.insert(j + 1);
      }
    }
    for (auto option : options) {
      for (int p = option; nodes[p].top > 0; p++) {
        cout << names[nodes[p].top] << " ";
      }
      cout << "\n";
    }
  }

  void dump_option(int p) {
    while (nodes[p].top > 0) {
      p--;
    }
    for (p = p + 1; nodes[p].top > 0; p++) {
      cout << names[nodes[p].top] << " ";
    }
  }

  int count_nodes(
      const vector<string>& items,
      const vector<vector<string>>& options) {
    int total_nodes = 2 + items.size();
    for (const auto& option : options) {
      total_nodes += 1 + option.size();
    }
    return total_nodes;
  }

  vector<Header> build_header(
      const vector<string>& items) {
    vector<Header> header(items.size() + 1);
    int size = items.size();
    for (int i = 0; i <= size; i++) {
      if (i == 0) {
        header[i] = { size, 1 };
      } else {
        header[i] = { i - 1, (i + 1) % (size + 1) };
      }
    }
    return header;
  }

  vector<Node> build_nodes(
      const vector<string>& items,
      const vector<vector<string>>& options) {
    vector<Node> nodes(count_nodes(items, options));
    vector<int> last(items.size() + 1);
    unordered_map<string, int> name;
    for (int i = 1; i <= int(items.size()); i++) {
      nodes[i] = {0, i, i};
      last[i] = i;
      name[items[i - 1]] = i;
    }
    int current = items.size() + 1;
    int lastspacer = -2;
    int spacer = current;
    for (int i = 0; i < int(options.size()); i++) {
      spacer = current;
      nodes[spacer].top = -i;
      nodes[spacer].ulink = lastspacer + 1;
      nodes[spacer].dlink = spacer + options[i].size();
      lastspacer = spacer;
      current++;
      for (int j = 0; j < int(options[i].size()); j++) {
        int item = name[options[i][j]];
        nodes[item].top++;
        nodes[last[item]].dlink = current;
        nodes[item].ulink = current;
        nodes[current].top = item;
        nodes[current].ulink = last[item];
        nodes[current].dlink = item;
        last[item] = current;
        current++;
      }
    }
    nodes[current].top = -int(options.size());
    nodes[current].ulink = lastspacer + 1;
    nodes[current].dlink = -1;
    return nodes;
  }

  vector<Header> header;
  vector<Node> nodes;
  const vector<string>& names;
  vector<int> solution;
  Graph graph;
  long count;
};

vector<string> parse_line(const string& line) {
  istringstream iss(line);
  vector<string> items;
  string item;
  while (iss >> item) {
    items.push_back(item);
  }
  return items;
}

int main() {
  string line;
  getline(cin, line);
  vector<string> items = parse_line(line);
  vector<vector<string>> options;
  while (getline(cin, line)) {
    vector<string> option = parse_line(line);
    if (!option.empty()) {
      options.push_back(option);
    }
  }
  ExactCover ec(items, options);
  //ec.print();
  //ec.write_dot();
  cout << "Number of solutions: " << ec.run() << "\n";
  return 0;
}
