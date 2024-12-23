/*
 * Copyright (c) 2007,2008,2009 INRIA, UDcast
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Authors: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *          Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                               <amine.ismail@UDcast.com>
 */

#ifndef CONNECTION_IDENTIFIER_FACTORY_H
#define CONNECTION_IDENTIFIER_FACTORY_H

#include "cid.h"

#include <stdint.h>

namespace ns3
{

/**
 * @ingroup wimax
 * This class is used exclusively by the BS to allocate CIDs to new connections.
 * This class uses the definitions from Table 345 in ieee-802.16-2004 to
 * allocate cids in different integer ranges according to their type.
 * The relative allocation of the ranges is controlled by the parameter 'm'
 * introduced in the above-mentioned table.
 *
 * Note: The current implementation merely allocates cids sequentially from
 * the start of the allowed range. A proper implementation would be able
 * to recycle cids from previous dead connections and should instead look
 * at a bitmap of free cids. Some other day :)
 */
class CidFactory
{
  public:
    /**
     * Create a cid factory with a default value for m of 0x5500.
     */
    CidFactory();
    /**
     * @brief This function returns the next basic CID.
     * @returns the next basic CID.
     */
    Cid AllocateBasic();
    /**
     * @brief This function returns the next primary basic CID.
     * @returns the next primary CID.
     */
    Cid AllocatePrimary();
    /**
     * @brief This function returns the next Transport (or Secondary) CID.
     * @returns the next Transport (or Secondary) CID.
     */
    Cid AllocateTransportOrSecondary();
    /**
     * @brief This function returns the next Multicast CID.
     * @returns the next Multicast CID.
     */
    Cid AllocateMulticast();
    /**
     * @brief This function returns the next CID for the specified type.
     * @param type CID type
     * @returns the next CID.
     */
    Cid Allocate(Cid::Type type);

    /**
     * @brief This function determines if the CID is a transport.
     * @param cid CID type
     * @returns true if the CID is a transport.
     */
    bool IsTransport(Cid cid) const;
    /**
     * @brief This function determines if the CID is primary.
     * @param cid CID type
     * @returns true if the CID is primary.
     */
    bool IsPrimary(Cid cid) const;
    /**
     * @brief This function determines if the CID is basic.
     * @param cid CID type
     * @returns true if the CID is basic.
     */
    bool IsBasic(Cid cid) const;

    /**
     * Notify the factory that the connection associated to this
     * cid has been killed and that this cid can be reused.
     * @param cid CID type
     */
    void FreeCid(Cid cid);

  private:
    uint16_t m_m; ///< m

    uint16_t m_basicIdentifier;                ///< basic identifier
    uint16_t m_primaryIdentifier;              ///< primary identifier
    uint16_t m_transportOrSecondaryIdentifier; ///< transport or secondary identifier
    uint16_t m_multicastPollingIdentifier;     ///< multicast polling identifier
};

} // namespace ns3

#endif /* CONNECTION_IDENTIFIER_FACTORY_H */
