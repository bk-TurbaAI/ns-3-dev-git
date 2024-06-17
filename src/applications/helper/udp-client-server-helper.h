/*
 * Copyright (c) 2008 INRIA
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */

#ifndef UDP_CLIENT_SERVER_HELPER_H
#define UDP_CLIENT_SERVER_HELPER_H

#include <ns3/application-helper.h>
#include <ns3/udp-client.h>
#include <ns3/udp-server.h>
#include <ns3/udp-trace-client.h>

#include <stdint.h>

namespace ns3
{
/**
 * \ingroup udpclientserver
 * \brief Create a server application which waits for input UDP packets
 *        and uses the information carried into their payload to compute
 *        delay and to determine if some packets are lost.
 */
class UdpServerHelper : public ApplicationHelper
{
  public:
    /**
     * Create UdpServerHelper which will make life easier for people trying
     * to set up simulations with udp-client-server application.
     *
     */
    UdpServerHelper();

    /**
     * Create UdpServerHelper which will make life easier for people trying
     * to set up simulations with udp-client-server application.
     *
     * \param port The port the server will wait on for incoming packets
     */
    UdpServerHelper(uint16_t port);
};

/**
 * \ingroup udpclientserver
 * \brief Create a client application which sends UDP packets carrying
 *  a 32bit sequence number and a 64 bit time stamp.
 *
 */
class UdpClientHelper : public ApplicationHelper
{
  public:
    /**
     * Create UdpClientHelper which will make life easier for people trying
     * to set up simulations with udp-client-server.
     *
     */
    UdpClientHelper();

    /**
     *  Create UdpClientHelper which will make life easier for people trying
     * to set up simulations with udp-client-server. Use this variant with
     * addresses that do not include a port value (e.g., Ipv4Address and
     * Ipv6Address).
     *
     * \param ip The IP address of the remote UDP server
     * \param port The port number of the remote UDP server
     */

    UdpClientHelper(const Address& ip, uint16_t port);
    /**
     *  Create UdpClientHelper which will make life easier for people trying
     * to set up simulations with udp-client-server. Use this variant with
     * addresses that do include a port value (e.g., InetSocketAddress and
     * Inet6SocketAddress).
     *
     * \param addr The address of the remote UDP server
     */

    UdpClientHelper(const Address& addr);
};

/**
 * \ingroup udpclientserver
 * Create UdpTraceClient application which sends UDP packets based on a trace
 * file of an MPEG4 stream. Trace files could be downloaded form :
 * https://web.archive.org/web/20190907061916/http://www2.tkn.tu-berlin.de/research/trace/ltvt.html
 * (the 2 first lines of the file should be removed)
 * A valid trace file is a file with 4 columns:
 * \li -1- the first one represents the frame index
 * \li -2- the second one indicates the type of the frame: I, P or B
 * \li -3- the third one indicates the time on which the frame was generated by the encoder
 * \li -4- the fourth one indicates the frame size in byte
 */
class UdpTraceClientHelper : public ApplicationHelper
{
  public:
    /**
     * Create UdpTraceClientHelper which will make life easier for people trying
     * to set up simulations with udp-client-server.
     *
     */
    UdpTraceClientHelper();

    /**
     * Create UdpTraceClientHelper which will make life easier for people trying
     * to set up simulations with udp-client-server. Use this variant with
     * addresses that do not include a port value (e.g., Ipv4Address and
     * Ipv6Address).
     *
     * \param ip The IP address of the remote UDP server
     * \param port The port number of the remote UDP server
     * \param filename the file from which packet traces will be loaded
     */
    UdpTraceClientHelper(const Address& ip, uint16_t port, const std::string& filename = "");

    /**
     * Create UdpTraceClientHelper which will make life easier for people trying
     * to set up simulations with udp-client-server. Use this variant with
     * addresses that do include a port value (e.g., InetSocketAddress and
     * Inet6SocketAddress).
     *
     * \param addr The address of the remote UDP server
     * \param filename the file from which packet traces will be loaded
     */
    UdpTraceClientHelper(const Address& addr, const std::string& filename = "");
};

} // namespace ns3

#endif /* UDP_CLIENT_SERVER_H */
