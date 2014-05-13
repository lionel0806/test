/*
 * atom.h
 *
 *  Created on: 2014Äê5ÔÂ12ÈÕ
 *      Author: lionel
 */

#ifndef ATOM_H_
#define ATOM_H_

const char *atom_new(const char *str, int len);
const char *atom_string(char *str);
const char *atom_int(long n);
int atom_length(const char *str);

#endif /* ATOM_H_ */
