#include<iostream>
#include<fstream>
#include<string>
#include<json.hpp>
#define inputdir "inputfiles/"
#define outputdir "outputfiles/"

using json = nlohmann::json;

void prettifyMessages(json& jin, json& jout) {
    for (int i = 0; i < jin.size(); i++) {
        for (int k = 0; k < jin[i]["conversation"].size(); k++) {
            json inmessage = jin[i]["conversation"][k], outmessage;

            std::string createdat, date, time;
            createdat = inmessage["created_at"];
            date = createdat.substr(0, 10);
            time = createdat.substr(11, 8);
            outmessage["date"] = date;
            outmessage["time"] = time;
            outmessage["sender"] = inmessage["sender"];
            if (inmessage.contains("media_owner")) {
                outmessage["media_owner"] = inmessage["media_owner"];
                outmessage["media_share_caption"] = inmessage["media_share_caption"];
                outmessage["media_share_url"] = inmessage["media_share_url"];
            }
            else
                outmessage["text"] = inmessage["text"];

            jout[i]["conversation"][k] = outmessage;
        }
        jout[i]["participants"] = jin[i]["participants"];
    }
}

void prettifySearches(json& jin, json& jout) {
    for (int i = 0; i < jin.size(); i++) {
        json inobj = jin[i], outobj = inobj;

        std::string intime, date, time;
        intime = inobj["time"];
        date = intime.substr(0, 10);
        time = intime.substr(11, 8);
        outobj["date"] = date;
        outobj["time"] = time;

        jout[i] = outobj;
    }
}

void prettifyLikes(json& jin, json& jout) {
    for (int i = 0; i < jin["comment_likes"].size(); i++) {
        json inobj = jin["comment_likes"][i], outobj;

        std::string intime, date, time;
        intime = inobj[0];
        date = intime.substr(0, 10);
        time = intime.substr(11, 8);
        outobj[0] = date;
        outobj[1] = time;
        outobj[2] = inobj[1];
 
        jout["comment_likes"][i] = outobj;
    }
    for (int i = 0; i < jin["media_likes"].size(); i++) {
        json inobj = jin["media_likes"][i], outobj;

        std::string intime, date, time;
        intime = inobj[0];
        date = intime.substr(0, 10);
        time = intime.substr(11, 8);
        outobj[0] = date;
        outobj[1] = time;
        outobj[2] = inobj[1];
 
        jout["media_likes"][i] = outobj;
    }
}

int main() {
    std::cout << std::endl << "This will read and output the contents of a JSON file";
    std::cout << std::endl << "Insert the name of the file (will look for it in inputdata/)(defaults to input.json): ";
    std::string infilename, outfilename;
    std::cin >> infilename;
    if (infilename.size() < 5 || infilename.substr(infilename.size() - 5, 5) != ".json")
        infilename = "input.json";
    outfilename = outputdir + infilename + ".txt";
    infilename = inputdir + infilename;
    
    //ensures file exists
    std::ofstream file(infilename, std::ios::app);
    file.close();

    json jin, jout;

    std::cout << std::endl << "Reading input...";
    //input data to jin from input file
    std::ifstream ifile(infilename);
    ifile >> jin;
    ifile.close();
    std::cout << std::endl << "...Reading completed";

    if (infilename == ((std::string)inputdir) + "messages.json") {
        prettifyMessages(jin, jout);
    }
    else if (infilename == ((std::string)inputdir) + "searches.json") {
        prettifySearches(jin, jout);
    }
    else if (infilename == ((std::string)inputdir) + "likes.json") {
        prettifyLikes(jin, jout);
    }
    else
        jout = jin;
    
    std::cout << std::endl << "Writing output...";
    //output data from jout to text file
    std::ofstream ofile(outfilename, std::ios::trunc);
    ofile << jout.dump(4) << std::endl;
    ofile.close();
    std::cout << std::endl << "...Writing completed";

    std::cout << std::endl << std::endl << "Press enter to exit the application";
    std::cin.get();
    std::cin.get();
    return 0;
}