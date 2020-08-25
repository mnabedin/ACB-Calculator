#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////

// Constructor
// TASK
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date, unsigned int month_date, unsigned year_date,
		bool buy_sell_trans, unsigned int number_shares, double trans_amount) {
	year = year_date, month = month_date, day = day_date;
	symbol = ticker_symbol;
	buy_sell_trans ? trans_type = "Buy" : trans_type = "Sell";
	shares = number_shares;
	amount = trans_amount;
	trans_id = assigned_trans_id;
	assigned_trans_id++;
	acb = 0.0;
	acb_per_share = 0.0;
	share_balance = 0;
	cgl = 0.0;
	p_next = nullptr;
}

// Destructor
// TASK
Transaction::~Transaction() {
}

// TASK
// Overloaded < operator.
bool Transaction::operator <(Transaction const &other) {
	if (this->year < other.year)
		return true;
	else if (this->year > other.year)
		return false;
	else if (this->year == other.year) {
		if (this->month < other.month)
			return true;
		else if (this->month > other.month)
			return false;
		else if (this->month == other.month) {
			if (this->day < other.day)
				return true;
			else if (this->day >= other.day)
				return false;
		}
	}
	return false;
}

// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const {
	return symbol;
}
unsigned int Transaction::get_day() const {
	return day;
}
unsigned int Transaction::get_month() const {
	return month;
}
unsigned int Transaction::get_year() const {
	return year;
}
unsigned int Transaction::get_shares() const {
	return shares;
}
double Transaction::get_amount() const {
	return amount;
}
double Transaction::get_acb() const {
	return acb;
}
double Transaction::get_acb_per_share() const {
	return acb_per_share;
}
unsigned int Transaction::get_share_balance() const {
	return share_balance;
}
double Transaction::get_cgl() const {
	return cgl;
}
bool Transaction::get_trans_type() const {
	return (trans_type == "Buy") ? true : false;
}
unsigned int Transaction::get_trans_id() const {
	return trans_id;
}
Transaction* Transaction::get_next() {
	return p_next;
}

// GIVEN
// Member functions to set values. 
void Transaction::set_acb(double acb_value) {
	acb = acb_value;
}
void Transaction::set_acb_per_share(double acb_share_value) {
	acb_per_share = acb_share_value;
}
void Transaction::set_share_balance(unsigned int bal) {
	share_balance = bal;
}
void Transaction::set_cgl(double value) {
	cgl = value;
}
void Transaction::set_next(Transaction *p_new_next) {
	p_next = p_new_next;
}

// GIVEN
// Print the transaction.
void Transaction::print() {
	std::cout << std::fixed << std::setprecision(2);
	std::cout << std::setw(4) << get_trans_id() << " " << std::setw(4) << get_symbol() << " " << std::setw(4)
			<< get_day() << " " << std::setw(4) << get_month() << " " << std::setw(4) << get_year() << " ";

	if (get_trans_type()) {
		std::cout << "  Buy  ";
	} else {
		std::cout << "  Sell ";
	}

	std::cout << std::setw(4) << get_shares() << " " << std::setw(10) << get_amount() << " " << std::setw(10)
			<< get_acb() << " " << std::setw(4) << get_share_balance() << " " << std::setw(10) << std::setprecision(3)
			<< get_acb_per_share() << " " << std::setw(10) << std::setprecision(3) << get_cgl() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////

// Constructor
// TASK
History::History() {
	p_head = nullptr;
}

// Destructor
// TASK
History::~History() {
	Transaction *p_trav { p_head }, *p_hold = nullptr;
	while (p_trav != nullptr) {
		p_hold = p_trav->get_next();
		delete p_trav;
		p_trav = p_hold;
	}
}

// TASK
// read_transaction(...): Read the transaction history from file. 
void History::read_history() {
	ece150::open_file();
	while (ece150::next_trans_entry() == true) {
		std::string t = ece150::get_trans_symbol();
		unsigned int d = ece150::get_trans_day(), m = ece150::get_trans_month(), y = ece150::get_trans_year(), num =
				ece150::get_trans_shares();
		bool bst = ece150::get_trans_type();
		double amt = ece150::get_trans_amount();
		Transaction *p_new = new Transaction(t, d, m, y, bst, num, amt);
		insert(p_new);
	}
	ece150::close_file();
}

// insert(...): Insert transaction into linked list.
void History::insert(Transaction *p_new_trans) {
	Transaction *p_trav { p_head };

	if (p_head == nullptr) {
		p_head = p_new_trans;
		return;
	}

	while (p_trav->get_next() != nullptr)
		p_trav = p_trav->get_next();
	p_trav->set_next(p_new_trans);
}

// TASK
// sort_by_date(): Sort the linked list by trade date.
void History::sort_by_date() {
	//get length of the linked list
	std::size_t len = 0;
	Transaction *p_trav { p_head };
	while (p_trav != nullptr) {
		p_trav = p_trav->get_next();
		len++;
	}

	//new sorted list
	Transaction *p_new { nullptr };
	for (std::size_t i = 0; i < len; ++i) {
		Transaction *p_hold { p_head->get_next() }, *p_edit = p_head;
		//add to front
		if (i == 0) {
			p_edit->set_next(p_new);
			p_new = p_edit;
		} else {
			if ((*p_edit) < (*p_new)) {
				p_edit->set_next(p_new);
				p_new = p_edit;
			} else {
				Transaction *p_temp { p_new };
				while (p_temp->get_next() != nullptr) {
					if ((*p_edit) < (*(p_temp->get_next())))
						break;
					p_temp = p_temp->get_next();
				}
				//link in the middle or at the end
				p_edit->set_next(p_temp->get_next());
				p_temp->set_next(p_edit);
			}
		}
		p_head = p_hold;
	}
	p_head = p_new;
	delete p_trav;
	p_trav = nullptr;
}

// TASK
// update_acb_cgl(): Updates the ACB and CGL values. 
void History::update_acb_cgl() {
	Transaction *p_trav { p_head };
	unsigned int shares = 0;
	double acb = 0;
	while (p_trav != nullptr) {
		if (p_trav->get_trans_type()) {
			acb += p_trav->get_amount();
			shares += p_trav->get_shares();
			p_trav->set_acb(acb);
			p_trav->set_share_balance(shares);
			p_trav->set_acb_per_share(acb / shares);
		} else {
			acb -= p_trav->get_shares() * (acb / shares);
			shares -= p_trav->get_shares();
			p_trav->set_acb(acb);
			p_trav->set_share_balance(shares);
			p_trav->set_acb_per_share(acb / shares);
			p_trav->set_cgl(p_trav->get_amount() - p_trav->get_shares() * acb / shares);
		}
		p_trav = p_trav->get_next();
	}
	delete p_trav;
	p_trav = nullptr;
}

// TASK
// compute_cgl(): )Compute the ACB, and CGL.
double History::compute_cgl(unsigned int year) {
	double cgl = 0;
	Transaction *p_trav { p_head };
	while (p_trav != nullptr) {
		if (p_trav->get_year() == year)
			cgl += p_trav->get_cgl();
		p_trav = p_trav->get_next();
	}
	delete p_trav;
	return cgl;
}

// TASK
// print() Print the transaction history.
void History::print() {
	std::cout << "========== BEGIN TRANSACTION HISTORY ==========" << std::endl;
	Transaction *p_trav { p_head };
	while (p_trav != nullptr) {
		p_trav->print();
		p_trav = p_trav->get_next();
	}
	std::cout << "========== END TRANSACTION HISTORY ==========" << std::endl;
}

// GIVEN
// get_p_head(): Full access to the linked list.
Transaction* History::get_p_head() {
	return p_head;
}
#endif
