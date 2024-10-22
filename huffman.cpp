  #include <cctype>
  #include <iostream>
  #include "huffmanNode.h"
  #include "minHeap.h"
  #include <fstream>
  #include <map>
  #include <stack>
  #include <stdint.h>
  #include <string>
  #include <unordered_map>

//Run this command to compile: g++ -shared -o libhuffman.dll huffman.cpp
extern "C" {
  typedef void (*ProgressCallback)(int);

  int total = 0;

  void compress(char* filepath, ProgressCallback callback) {
      string str = "";
      char ch;
      ifstream fin;
      fin.open(string(filepath));
      while(fin.get(ch))
        str += ch;
      fin.close();
      
      map<int, int> countMap;
      for (int i = 0; i < str.length(); i++) {
        int c = int(str[i]);
        countMap[c]++;
        total++;
      }

      minHeap<node> h(countMap.size());
      
      for (map<int, int>::iterator it = countMap.begin(); it != countMap.end(); it++) {
        node n = {it->first, it->second, nullptr, nullptr};
        h.insert(n);
      }
      
      while (h.getNum() > 1) {
        node *left_child = new node(h.getMin());
        node *right_child = new node(h.getMin());
        node new_node = {-1, left_child->count + right_child->count, left_child, right_child};
        h.insert(new_node);
      }
      
      stack<node> s;
      stack<string> codes;
      map<int, string> codeMap;

      s.push(h.getMin());
      codes.push("");

      while (!s.empty()) {
        node p = s.top();
        s.pop();

        string current_code = codes.top();
        codes.pop();

        if (p.ascii != -1) {
          codeMap[p.ascii] = current_code;
        }

        if (p.right != NULL) {
          s.push(*p.right);
          codes.push(current_code + "1");
        }

        if (p.left != NULL) {
          s.push(*p.left);
          codes.push(current_code + "0");
        }
      }

      //cout << endl << "Compressed text: ";
      ofstream fout;
      fout.open("compressed.out", ios::binary | ios::out | ios::app);

      for (map<int, int>::iterator it = countMap.begin(); it != countMap.end(); it++) {
        if (next(it) == countMap.end()) {
          //cout << it->first << "," << it->second << "|";
          fout << it->first << "," << it->second << "|";
        }
        else {
        //cout << it->first << "," << it->second << ";";
        fout << it->first << "," << it->second << ";";
        }
      }

      callback(0);
      int counter = 0;
      int interval = total / 1000;
      float progress = 0;
      string str2 = "";
      for (int i = 0; i < str.length(); i++) {
        int c = int(str[i]);
        //cout << codeMap[c];
        str2 += codeMap[c];

        counter++;
        if(counter >= interval) {
          counter = 0;
          progress++;
          if (progress <= 1000) {
            callback(progress);
            }
        }
      }

      uint8_t byte = 0;
        int pos = 0;
        for (int i = 0; i < str2.size(); i++) {
            if (str2[i] == '1') {
                byte |= (1 << (7 - pos));
            }
            pos++;
            if (pos == 8 || i == str2.size() - 1) {
                fout << byte;
                byte = 0;
                pos = 0;
            }
        }
      
        fout.close();

        callback(1000);
  }
  void decompress(ProgressCallback callback) {
      ifstream fin;
      ofstream fout;
      fin.open("compressed.out", ios::binary | ios::in);
      fout.open("decompressed.out");
      map<int, int> countMap2;
      int ascii;
      int count;
      char sep1, sep2 = ' ';

      while (sep2 != '|' && fin >> ascii >> sep1 >> count >> sep2)
        countMap2[ascii] = count;

      minHeap<node> h2(countMap2.size());

      for (map<int, int>::iterator it = countMap2.begin(); it != countMap2.end(); it++) {
        node n = {it->first, it->second, nullptr, nullptr};
        h2.insert(n);
      }
      
      while (h2.getNum() > 1) {
        node *left_child = new node(h2.getMin());
        node *right_child = new node(h2.getMin());
        node new_node = {-1, left_child->count + right_child->count, left_child, right_child};
        h2.insert(new_node);
      }
      
      stack<node> s2;
      stack<string> codes2;
      map<string, int> codeMap2;

      s2.push(h2.getMin());
      codes2.push("");

      cout << endl << endl;
      while (!s2.empty()) {
        node p = s2.top();
        s2.pop();

        string current_code = codes2.top();
        codes2.pop();

        if (p.ascii != -1) {
          codeMap2[current_code] = p.ascii;
          //cout << p.ascii << " " << current_code << endl;
        }

        if (p.right != NULL) {
          s2.push(*p.right);
          codes2.push(current_code + "1");
        }

        if (p.left != NULL) {
          s2.push(*p.left);
          codes2.push(current_code + "0");
        }
      }

      char ch2;
      string bits = "";
      while (fin.get(ch2)) {
        string b2 = "";
        for (int i = 7; i >= 0; i--) {
          if (ch2 & (1 << i)) {
            b2.append("1");
          } 
          else {
            b2.append("0");
          }
        }
        bits.append(b2);
      }
      fin.close();

      //cout << endl << "Decoded text: ";
      callback(0);
      int counter = 0;
      int interval = total / 1000;
      float progress = 0;
      string prevCode = "";
      string code = "";
      for (int i = 0; i < bits.size(); i++) {
        prevCode = code;
        code += bits[i];
        if (codeMap2.count(prevCode) > 0 && codeMap2.count(code) == 0) {
          //cout << char(codeMap2[prevCode]);
          fout << char(codeMap2[prevCode]);
          code = bits[i];

          counter++;
        if(counter >= interval) {
          counter = 0;
          progress++;
          if (progress <= 1000) {
            callback(progress);
          }
        }
        }
      }
      fout.close();

      callback(1000);
  }
}
