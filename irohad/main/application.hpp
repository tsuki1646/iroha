/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_APPLICATION_HPP
#define IROHA_APPLICATION_HPP

#include "ametsuchi/impl/storage_impl.hpp"
#include "logger/logger.hpp"
#include "main/impl/block_loader_init.hpp"
#include "main/impl/consensus_init.hpp"
#include "main/impl/ordering_init.hpp"
#include "main/server_runner.hpp"
#include "model/model_crypto_provider_impl.hpp"
#include "network/block_loader.hpp"
#include "network/consensus_gate.hpp"
#include "network/ordering_gate.hpp"
#include "network/peer_communication_service.hpp"
#include "simulator/block_creator.hpp"
#include "simulator/impl/simulator.hpp"
#include "synchronizer/synchronizer.hpp"
#include "torii/command_service.hpp"
#include "torii/processor/query_processor_impl.hpp"
#include "torii/processor/transaction_processor_impl.hpp"
#include "validation/chain_validator.hpp"
#include "validation/impl/stateless_validator_impl.hpp"
#include "validation/stateful_validator.hpp"

#include "ametsuchi/impl/peer_query_wsv.hpp"
#include "network/impl/peer_communication_service_impl.hpp"
#include "synchronizer/impl/synchronizer_impl.hpp"
#include "validation/impl/chain_validator_impl.hpp"
#include "validation/impl/stateful_validator_impl.hpp"

class Irohad {
 public:
  /**
   * Constructor that initializes common iroha pipeline
   * @param block_store_dir - folder where blocks will be stored
   * @param redis_host - host of redis connection
   * @param redis_port - port of redis connection
   * @param pg_conn - initialization string for postgre
   * @param torii_port - port for torii binding
   * @param internal_port - port for internal communication - ordering service,
   * consensus, and block loader
   * @param keypair - public and private keys for crypto provider
   */
  Irohad(const std::string &block_store_dir,
         const std::string &redis_host,
         size_t redis_port,
         const std::string &pg_conn,
         size_t torii_port,
         size_t internal_port,
         const iroha::keypair_t &keypair);

  /**
   * Initialization of whole objects in system
   */
  virtual void init();

  /**
   * Drop wsv and block store
   */
  virtual void dropStorage();

  /**
   * Run worker threads for start performing
   */
  virtual void run();

  virtual ~Irohad();

 protected:
  // -----------------------| component initialization |------------------------

  virtual void initStorage();

  virtual void initProtoFactories();

  virtual void initPeerQuery();

  virtual void initCryptoProvider();

  virtual void initValidators();

  virtual void initOrderingGate();

  virtual void initSimulator();

  virtual void initBlockLoader();

  virtual void initConsensusGate();

  virtual void initSynchronizer();

  virtual void initPeerCommunicationService();

  virtual void initTransactionCommandService();

  virtual void initQueryService();

  // constructor dependencies
  std::string block_store_dir_;
  std::string redis_host_;
  size_t redis_port_;
  std::string pg_conn_;
  size_t torii_port_;
  size_t internal_port_;

  // ------------------------| internal dependencies |-------------------------

  // converter factories
  std::shared_ptr<iroha::model::converters::PbTransactionFactory> pb_tx_factory;
  std::shared_ptr<iroha::model::converters::PbQueryFactory> pb_query_factory;
  std::shared_ptr<iroha::model::converters::PbQueryResponseFactory>
      pb_query_response_factory;

  // crypto provider
  std::shared_ptr<iroha::model::ModelCryptoProvider> crypto_verifier;

  // validators
  std::shared_ptr<iroha::validation::StatelessValidator> stateless_validator;
  std::shared_ptr<iroha::validation::StatefulValidator> stateful_validator;
  std::shared_ptr<iroha::validation::ChainValidator> chain_validator;

  // peer query
  std::shared_ptr<iroha::ametsuchi::PeerQuery> wsv;

  // ordering gate
  std::shared_ptr<iroha::network::OrderingGate> ordering_gate;

  // simulator
  std::shared_ptr<iroha::simulator::Simulator> simulator;

  // block loader
  std::shared_ptr<iroha::network::BlockLoader> block_loader;

  // consensus gate
  std::shared_ptr<iroha::network::ConsensusGate> consensus_gate;

  // synchronizer
  std::shared_ptr<iroha::synchronizer::Synchronizer> synchronizer;

  // pcs
  std::shared_ptr<iroha::network::PeerCommunicationService> pcs;

  // transaction service
  std::unique_ptr<torii::CommandService> command_service;

  // query service
  std::unique_ptr<torii::QueryService> query_service;

  std::unique_ptr<ServerRunner> torii_server;
  std::unique_ptr<grpc::Server> internal_server;

  // initialization objects
  iroha::network::OrderingInit ordering_init;
  iroha::consensus::yac::YacInit yac_init;
  iroha::network::BlockLoaderInit loader_init;

  std::thread internal_thread, server_thread;

  logger::Logger log_;

 public:
  std::shared_ptr<iroha::ametsuchi::Storage> storage;
  iroha::keypair_t keypair;
};

#endif  // IROHA_APPLICATION_HPP
