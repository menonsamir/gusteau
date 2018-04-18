#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <cstdint>
#include <map>
#include <deque>

/* Congestion controller interface */

class Controller
{
private:
  bool debug_; /* Enables debugging output */

  /* Add member variables here */
  uint64_t acks_recvd;
  double cur_win;
  uint64_t intersend;

  uint64_t last_ack;
  double r_ewma; // moving average of interval between acks
  double r_slowewma; // slower moving average of interval between acks

  uint64_t last_send;
  double s_ewma; // moving average between sender timestamps echoed in acks

  uint64_t min_rtt; // smallest RTT so far
  double rtt_ratio; // ratio of last RTT to smallest RTT so far
  /*double rtt_ewma;
  double v;
  double delta_inv;
  std::deque<std::pair<double, double> > rtts;
  //std::map<uint64_t, double> packetno_to_win;
  double last_win;
  bool last_dir;
  bool last_last_dir;
  double rtt_standing;*/

public:
  /* Public interface for the congestion controller */
  /* You can change these if you prefer, but will need to change
     the call site as well (in sender.cc) */

  /* Default constructor */
  Controller( const bool debug );

  /* Get current window size, in datagrams */
  unsigned int window_size();
  
  /* Applies the Remy rules */
  bool apply_rules();
  double min_over(double current_time, double ago);

  /* A datagram was sent */
  void datagram_was_sent( const uint64_t sequence_number,
        const uint64_t send_timestamp,
        const bool after_timeout );

  /* An ack was received */
  void ack_received( const uint64_t sequence_number_acked,
         const uint64_t send_timestamp_acked,
         const uint64_t recv_timestamp_acked,
         const uint64_t timestamp_ack_received );

  /* How long to wait (in milliseconds) if there are no acks
     before sending one more datagram */
  unsigned int timeout_ms();
};

#endif
