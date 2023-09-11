#include <bits/stdc++.h>
using namespace std;

bool verbose;
string filename;

void check(bool cond, const string& hint, int line) {
  if (!cond) {
    cerr << "syntax error";
    if (verbose) {
      cerr << ": " << filename;
      if (line) cerr << ':' << line;
      cerr << ' ' << hint;
    }
    cerr << endl;
    ::exit(3);
  }
}

int digit(int n) {
  return to_string(abs(n)).length();
}

class tape {
private:
  int pos = 0;
  vector<char> positive;
  vector<char> negative;

  int get_min_pos() {
    for (int i = negative.size() - 1; i > 0; --i) {
      if (negative[i] != 0) {
        return -i;
      } else {
        negative.pop_back();
      }
    }
    for (int i = 0, j = positive.size(); i < j; ++i) {
      if (positive[i] != 0) {
        return i;
      }
    }
    return 2147483647;
  }

  int get_max_pos() {
    for (int i = positive.size() - 1; i >= 0; --i) {
      if (positive[i] != 0) {
        return i;
      } else {
        positive.pop_back();
      }
    }
    for (int i = 1, j = negative.size(); i < j; ++i) {
      if (negative[i] != 0) {
        return -i;
      }
    }
    return -2147483648;
  }

  char& get(int i) {
    vector<char> &half_tape = i >= 0 ? positive : negative;
    int abspos = abs(i);
    if (half_tape.size() <= abspos) {
      half_tape.resize(abspos + 1);
    }
    return half_tape[abspos];
  }

public:
  void init(const string& str) {
    for (char ch: str) {
      positive.push_back(ch);
    }
  }

  char& curr() {
    return get(pos);
  }

  void move(int direct) {
    pos += direct;
  }

  void print_index() {
    int min_pos = min(pos, get_min_pos()), max_pos = max(pos, get_max_pos());
    for (int i = min_pos; i <= max_pos; ++i) {
      cout << abs(i) << ' ';
    }
    cout << endl;
  }

  void print_tape() {
    int min_pos = min(pos, get_min_pos()), max_pos = max(pos, get_max_pos());
    for (int i = min_pos; i <= max_pos; ++i) {
      cout << left << setw(digit(i) + 1) << (get(i) ? get(i) : '_');
    }
    cout << endl;
  }

  void print_head() {
    int min_pos = min(pos, get_min_pos()), max_pos = max(pos, get_max_pos());
    for (int i = min_pos; i <= max_pos; ++i) {
      cout << left << setw(digit(i) + 1) << (i == pos ? '^' : ' ');
    }
    cout << endl;
  }

  void print_res() {
    int min_pos = get_min_pos(), max_pos = get_max_pos();
    for (int i = min_pos; i <= max_pos; ++i) {
      cout << (get(i) ? get(i) : '_');
    }
    cout << endl;
  }
};

vector<string> status;
unordered_map<string, int> status_lut;
unordered_set<char> input_syms;
unordered_set<char> tape_syms;
int curr_status;
unordered_set<int> final_status;
int tape_num;
vector<tape> tapes;

class rule {
private:
  int from_status, to_status;
  string old_sym, new_sym, move;

public:
  rule(const string& str, int line) {
    stringstream buf(str);
    string status1, status2;
    buf >> status1 >> old_sym >> new_sym >> move >> status2;
    check(buf && buf.eof(), "illegal line format", line);
    check(status_lut.count(status1), "undefined from state", line);
    check(status_lut.count(status2), "undefined to state", line);
    check(old_sym.length() == tape_num, "old symbol length not match", line);
    check(new_sym.length() == tape_num, "new symbol length not match", line);
    check(move.length() == tape_num, "move length not match", line);
    from_status = status_lut[status1];
    to_status = status_lut[status2];
    for (int i = 0; i < tape_num; ++i) {
      if (old_sym[i] == '_') old_sym[i] = 0;
      else check(old_sym[i] == '*' || tape_syms.count(old_sym[i]), 
                 "illegal old symbol", line);
      if (new_sym[i] == '_') new_sym[i] = 0;
      else check(new_sym[i] == '*' || tape_syms.count(new_sym[i]), 
                 "illegal new symbol", line);
      if (new_sym[i] == '*') check(old_sym[i] == '*', "illegal star", line);
      check(move[i] == 'r' || move[i] == 'l' || move[i] == '*', "illegal move", line);
    }
  }

  int old_status() {
    return from_status;
  }

  bool match() {
    for (int i = 0; i < tape_num; ++i) {
      char sym = tapes[i].curr();
      if ((old_sym[i] == '*' && sym == 0) || (old_sym[i] != '*' && old_sym[i] != sym))
        return false;
    }
    return true;
  }

  void apply() {
    for (int i = 0; i < tape_num; ++i) {
      if (new_sym[i] != '*') {
        tapes[i].curr() = new_sym[i];
      }
      switch (move[i]) {
      case 'l': tapes[i].move(-1); break;
      case 'r': tapes[i].move(1); break;
      }
    }
    curr_status = to_status;
  }
};

vector<vector<rule>> rules;

void print_turing(int step) {
  cout << left << setw(6 + digit(tape_num)) << "Step" << ": " << step << endl;
  cout << left << setw(6 + digit(tape_num)) << "State" << ": " 
       << status[curr_status] << endl;
  for (int i = 0; i < tape_num; ++i) {
    cout << "Index" << left << setw(1 + digit(tape_num)) << i << ": ";
    tapes[i].print_index();
    cout << "Tape" << left << setw(2 + digit(tape_num)) << i << ": ";
    tapes[i].print_tape();
    cout << "Head" << left << setw(2 + digit(tape_num)) << i << ": ";
    tapes[i].print_head();
  }
  cout << "---------------------------------------------" << endl;
}

void do_turing() {
  int step = 0;
  if (verbose) print_turing(step);
  while (true) {
    bool found = false;
    for (rule &rl: rules[curr_status]) {
      if (rl.match()) {
        found = true;
        rl.apply();
        break;
      }
    }
    if (!found) break;
    ++step;
    if (verbose) print_turing(step);
  }
}

vector<string> split(const string& str) {
  vector<string> res;
  stringstream ss(str);
  for (string s; getline(ss, s, ','); ) {
    if (!s.empty()) res.push_back(s);
  }
  return res;
}

void read_file() {
  ifstream fs(filename);
  if (!fs) {
    cerr << filename << ": no such file" << endl;
    ::exit(2);
  }
  pair<string, int> q, s, g, q0, f;
  vector<pair<string, int>> delta;
  int flag = 0, linenum = 1;
  for (string line; getline(fs, line); ++linenum) {
    stringstream ss;
    ss << line;
    getline(ss >> ws, line);
    size_t comment_pos = line.find(';');
    if (comment_pos != string::npos) line.erase(comment_pos);
    while (!line.empty() && isspace(line.back())) line.pop_back();
    if (line.empty()) continue;
    if (line[0] == '#') {
      pair<string, int> *part;
      check(line.length() >= 6, "illegal line format", linenum);
      switch (line[1]) {
      case 'Q': check((flag & 1) == 0, "redifine states", linenum); flag |= 1;
                part = &q; goto L0;
      case 'S': check((flag & 2) == 0, "redifine input symbols", linenum); flag |= 2;
                part = &s; goto L0;
      case 'G': check((flag & 4) == 0, "redifine tape symbols", linenum); flag |= 4;
                part = &g; goto L0;
      case 'F': check((flag & 8) == 0, "redifine final states", linenum); flag |= 8;
                part = &f;
      L0:       check(line.length() >= 7 && line.substr(2, 4) == " = {" &&
                      line.back() == '}', "illegal line format", linenum); 
                part->first = line.substr(6, line.length() - 7);
                part->second = linenum; break;
      case 'B': check((flag & 16) == 0, "redifine blank symbol", linenum); flag |= 16; 
                check(line == "#B = _", "illegal blank symbol", linenum); break;
      case 'N': check((flag & 32) == 0, "redifine tape number", linenum); flag |= 32; 
                check(line.substr(0, 5) == "#N = ", "illegal line format", linenum);
                tape_num = stoi(line.substr(5));
                check(tape_num > 0, "illegal tape number", linenum);
                check(line.substr(5) == to_string(tape_num), 
                      "illegal line format", linenum); break;
      case 'q': check((flag & 64) == 0, "redifine start states", linenum); flag |= 64;
                check(line.length() >= 7 && line.substr(0, 6) == "#q0 = ",
                      "illegal line format", linenum);
                q0.first = line.substr(6); q0.second = linenum; break;
      default: check(false, "illegal line format", linenum);
      }
    } else {
      delta.emplace_back(line, linenum);
    }
  }
  check(flag == 127, "missing "s + 
        &("state\0\0\0\0\0\0\0\0" "input symbol\0" "tape symbol\0\0" "final state\0\0"
          "blank symbol\0" "tape number\0\0" "start state\0\0")
        [(__builtin_ffs(~flag) - 1) * 13], 0);
  tapes.resize(tape_num);
  for (string qs: split(q.first)) {
    for (char ch: qs) {
      check(isalnum(ch) || ch == '_', "illegal state name", q.second);
    }
    check(!status_lut.count(qs), "duplicate state name", q.second);
    status.push_back(qs);
    status_lut[qs] = status.size() - 1;
  }
  rules.resize(status.size());
  for (string gs: split(g.first)) {
    check(gs.length() == 1, "illegal tape symbol", g.second);
    char ch = gs[0];
    check(ch != ' ' && ch != ',' && ch != ';' && ch != '{' && ch != '}' && ch != '*',
          "illegal tape symbol", g.second);
    check(!tape_syms.count(ch), "duplicate tape symbol", g.second);
    tape_syms.insert(ch);
  }
  check(tape_syms.count('_'), "blank not in tape symbol", g.second);
  for (string ss: split(s.first)) {
    check(ss.length() == 1, "illegal input symbol", s.second);
    char ch = ss[0];
    check(ch != ' ' && ch != ',' && ch != ';' && ch != '{' && ch != '}' && ch != '*' && 
          ch != '_', "illegal input symbol", s.second);
    check(tape_syms.count(ch), "input symbol not in tape symbol", s.second);
    check(!input_syms.count(ch), "duplicate input symbol", s.second);
    input_syms.insert(ch);
  }
  for (string fs: split(f.first)) {
    check(status_lut.count(fs), "undefined final state", f.second);
    check(!final_status.count(status_lut[fs]), "duplicate final state", f.second);
    final_status.insert(status_lut[fs]);
  }
  check(status_lut.count(q0.first), "undefined start state", q0.second);
  curr_status = status_lut[q0.first];
  for (pair<string, int> &rs: delta) {
    rule rl(rs.first, rs.second);
    rules[rl.old_status()].push_back(rl);
  }
}

int main(int argc, char *argv[]) {
  bool help = false, err = false;
  string input;
  for (int i = 1; i < argc; ++i) {
    if (argv[i] == "-h"s || argv[i] == "--help"s) {
      help = true;
    } else if (argv[i] == "-v"s || argv[i] == "--verbose"s) {
      verbose = true;
    } else if (filename.empty()) {
      filename = argv[i];
    } else if (input.empty()) {
      input = argv[i];
    } else {
      err = true;
    }
  }
  if (filename.empty()) err = true;
  if (help) {
    cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
    return 0;
  } else if (err) {
    cerr << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
    return 1;
  }
  read_file();
  for (int i = 0, j = input.length(); i < j; ++i) {
    if (!input_syms.count(input[i])) {
      if (verbose) {
        cerr << "Input: " << input << endl;
        cerr << "==================== ERR ====================" << endl;
        cerr << "error: '" << input[i] << "' was not declared in the set of input symbols"
             << endl;
        cerr << "Input: " << input << endl;
        cerr << right << setw(8 + i) << '^' << endl;
        cerr << "==================== END ====================" << endl;
      } else {
        cerr << "illegal input" << endl;
      }
      return 4;
    }
  }
  tapes[0].init(input);
  if (verbose) {
    cout << "Input: " << input << endl;
    cout << "==================== RUN ====================" << endl;
  }
  do_turing();
  if (verbose) {
    cout << "Result: ";
    tapes[0].print_res();
    cout << "==================== END ====================" << endl;
  } else tapes[0].print_res();
  return 0;
}
