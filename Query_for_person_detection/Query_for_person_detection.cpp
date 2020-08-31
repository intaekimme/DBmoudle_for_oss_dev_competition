#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <future>

using namespace std;

MYSQL* conn, mysql;
MYSQL_RES* res;
MYSQL_ROW row;

int query_state;

void send_select(string table)
{
    string qsq = "select * from " + table;
    const char* query_state_query = qsq.c_str(); // query_state


    // db 상태 확인
    query_state = mysql_query(conn, query_state_query);
    if (query_state != 0) {
        std::cout << mysql_error(conn) << endl << endl;
        return;
    }

    res = mysql_store_result(conn);
    cout << " MySQL Tables in mysql database." << endl << endl;
    while ((row = mysql_fetch_row(res)) != NULL) {
        cout << left;
        cout << setw(18) << row[0]
            << setw(18) << row[1]
            << setw(18) << row[2]
            << setw(18) << row[3]
            << setw(18) << row[4]
            << setw(18) << row[5] << endl;
    } // follow amount of colum.
    cout << endl << endl;
}

//void send_insert_to_video_one(int frame, int id, int bb_left, int bb_top, int bb_width, int bb_height)
//{
//   string msg = "INSERT INTO video_one (frame, id, bb_left, bb_top, bb_width, bb_height) VALUES('" + to_string(frame) + "', '" + to_string(id) + "', '" + to_string(bb_left) + "', '" + to_string(bb_top) + "', '" + to_string(bb_width) + "', '" + to_string(bb_height) + "')";
//   const char* query = msg.c_str(); // query_state
//   query_state = mysql_query(conn, query);
//   if (query_state != 0) {
//      cout << mysql_error(conn) << endl << endl;
//      return;
//   }
//}
//
//void file_read(string txtfile)
//{
//   ifstream infile;
//   infile.open(txtfile.c_str());
//   if (infile.fail()) {
//      cout << "ERROR. Could not open file!" << endl;
//      return;
//   }
//
//   infile.seekg(0, infile.end);
//   int len = infile.tellg();
//   infile.seekg(0, infile.beg);
//   string frame, id, left, top, w, h;
//   string rating;
//   //string msg = "INSERT INTO "
//
//   while (infile)
//   {
//      infile >> frame;
//      cout << frame << endl;
//
//      infile >> id;
//      cout << id << endl;
//
//      infile >> left;
//      cout << left << endl;
//
//      infile >> top;
//      cout << top << endl;
//
//      infile >> w;
//      cout << w << endl;
//
//      infile >> h;
//      cout << h << endl;
//
//      send_insert_to_video_one(stoi(frame), stoi(id), stoi(left), stoi(top), stoi(w), stoi(h));
//
//   }
//}
//
//void signal_cheking()
//{
//   //select
//   string q = "select * from signal";
//   const char* query = q.c_str(); // query_state
//
//
//   // db 상태 확인
//   query_state = mysql_query(conn, query);
//   if (query_state != 0) {
//      std::cout << mysql_error(conn) << endl << endl;
//      return;
//   }
//
//   res = mysql_store_result(conn);
//   cout << " MySQL Tables in mysql database." << endl << endl;
//   while ((row = mysql_fetch_row(res)) != NULL) {
//      cout << left;
//      cout << setw(18) << row[0] << endl;
//   } // follow amount of colum.
//   cout << endl << endl;
//
//   //if
//   if (row[0] == "1") {
//      file_read("testvideolog.txt");
//   }
//
//   //신호지우기
//
//   //file_read
//
//   //
//}

////

void send_insert_to_db(string table, vector<string> columns, vector<string> values)
{
    //string msg = "INSERT INTO "+ table + " (frame, id, age, gender, is_male, has_bag, has_bagpack, has_hat, has_longsleeves, has_pants, has_longhair, has_coatjacket) VALUES('" + to_string(frame) + "', '" + to_string(id) + "', '" + to_string(bb_left) + "', '" + to_string(bb_top) + "', '" + to_string(bb_width) + "', '" + to_string(bb_height) + "')";

    string msg_insert = "INSERT INTO " + table + " (";
    for (int i = 0; i < columns.size() - 1; i++) {
        msg_insert.append(columns[i]);
        msg_insert.append(", ");
    }
    msg_insert.append(columns[columns.size() - 1]);
    msg_insert.append(") values(");
    for (int i = 0; i < values.size() - 1; i++) {
        msg_insert.append(values[i]);
        msg_insert.append(", ");
    }
    msg_insert.append(values[values.size() - 1]);
    msg_insert.append(")");

    std::cout << endl << msg_insert << endl;

    const char* query = msg_insert.c_str(); // query_state
    query_state = mysql_query(conn, query);
    if (query_state != 0) {
        std::cout << mysql_error(conn) << endl << endl;
        return;
    }
}

// reading text log file and send INSERT command with information of it.
void readFile(string txtfile, string table, vector<string> columns, int columnSize)
{
    ifstream infile;
    infile.open(txtfile.c_str());
    if (infile.fail()) {
        std::cout << "ERROR. Could not open file!" << endl;
        return;
    }
    infile.seekg(0, infile.end);
    int len = infile.tellg();
    infile.seekg(0, infile.beg);
    vector<string> contents;

    // split and send "INSERT" command.
    while (infile) {
        contents.clear();
        string str = "'";
        infile >> str;
        //str.append("'");
        contents.push_back(str);
        send_insert_to_db(table, columns, contents);
    }
}

int main(void)
{
    // connect mysql
    const char* server = "localhost";
    const char* user = "root";
    const char* password = "chrlghk1";
    const char* database = "video_detector";
    std::cout << mysql_get_client_info();

    const char* query_state_query = "select * from table_videoanalyzing"; // query_state

    // db 연결 및 에러확인
    mysql_init(&mysql);
    conn = mysql_real_connect(&mysql, server, user, password, database, 3306, 0, 0);
    if (conn == NULL) {
        std::cout << mysql_error(&mysql) << endl << endl;
        return 1;
    }

    // 비동기
    //future<void> check = std::async(launch::async, signal_cheking);

    // set colums
    vector<string> columns;
    columns.push_back("id");            //1: id
    columns.push_back("frame");            //2: frame
    columns.push_back("millisec");         //3: millisec
    columns.push_back("age");
    columns.push_back("gender");
    columns.push_back("is_male");
    columns.push_back("has_bag");
    columns.push_back("has_backpack");
    columns.push_back("has_hat");
    columns.push_back("has_longsleeves");   //10: has_longsleeves
    columns.push_back("has_longpants");
    columns.push_back("has_longhair");
    columns.push_back("has_coatjacket");
    columns.push_back("img_person");
    columns.push_back("img_face");
    columns.push_back("top_color");
    columns.push_back("bottom_color");      //17: bottom_color

    // txtfile -> mysql
    readFile("videolog.txt", "table_videoanalyzing", columns, 17);


    mysql_free_result(res);
    mysql_close(conn);
    std::cout << "done" << endl;



    return 0;
}