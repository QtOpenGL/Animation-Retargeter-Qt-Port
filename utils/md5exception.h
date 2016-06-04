#ifndef MD5EXCEPTION_H
#define MD5EXCEPTION_H

/*
 * MD5Exception
 * Based on the work by David Henry
 * http://tfc.duke.free.fr/coding/md5-specs-en.html
 */
class MD5Exception: public std::exception{

private:
    string issue, filepath;

public:
    MD5Exception(string issue, string filepath){ this->issue = issue; this->filepath = filepath;}
    MD5Exception(string issue){ this->issue = issue; this->filepath = "";}

    string getMessage(){ return issue + " with " + filepath; }

};

#endif // MD5EXCEPTION_H
