// Copyright (c) 2018 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENTXS_CORE_CONTRACT_PEER_BAILMENTREQUEST_HPP
#define OPENTXS_CORE_CONTRACT_PEER_BAILMENTREQUEST_HPP

#include "opentxs/Forward.hpp"

#include "opentxs/core/contract/peer/PeerRequest.hpp"

namespace opentxs
{

class BailmentRequest : public PeerRequest
{
public:
    ~BailmentRequest() = default;

private:
    using ot_super = PeerRequest;
    friend class PeerRequest;

    OTIdentifier unit_;
    OTIdentifier server_;

    proto::PeerRequest IDVersion(const Lock& lock) const override;

    BailmentRequest(
        const api::Wallet& wallet,
        const ConstNym& nym,
        const proto::PeerRequest& serialized);
    BailmentRequest(
        const api::Wallet& wallet,
        const ConstNym& nym,
        const Identifier& recipientID,
        const Identifier& unitID,
        const Identifier& serverID);
    BailmentRequest() = delete;
};
}  // namespace opentxs

#endif
