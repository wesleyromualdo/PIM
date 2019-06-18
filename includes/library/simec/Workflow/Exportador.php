<?php

initAutoload();

/**
 * Class Simec_Workflow_Exportador
 * Implementação do Simec_Exportador_ExportadorAbstrato para importação/exportação de workflow
 * @author ledsonsilva
 *
 */
class Simec_Workflow_Exportador extends Simec_Exportador_ExportadorAbstrato
{
	private $id;
	private $dados = array();
	
	public  $log;
	
	const	LOG_ESTADO_CRIADO 	= "[Estado] <b>\"%s\"</b> Criado";
	const	LOG_ESTADO_ALTERADO = "[Estado] <b>\"%s\"</b> Teve a(o) %s alterada(o) para <b>\"%s\"</b>";
	const	LOG_ESTADO_DELETADO	= "[Estado] <b>\"%s\"</b> Foi desativado";
	
	const	LOG_ACAO_CRIADO		= "[Ação] <b>\"%s\"</b> Criada";
	const	LOG_ACAO_ALTERADA	= "[Ação] <b>\"%s\"</b> Teve a(o) %s alterada(o) para <b>\"%s\"</b>";
	const	LOG_ACAO_PERFIL		= "[Ação] <b>\"%s\"</b> Teve o perfil <b>\"%s\"</b> atribuido";
	const	LOG_ACAO_DELETADA	= "[Ação] <b>\"%s\"</b> Foi desativada";
	
	/**
	 * Construtor
	 * @param string $id
	 */
	public function __construct($id = null)
	{
		if($id) {
			$this->id = $id;
		}

		$this->log = new Simec_Exportador_Log();
	}
	
	/**
	 * Gera Hash e prepara dados para exportação
	 * @return array
	 */
	public function exportar()
	{
		// Gera hashid
		$this->gerarHashTipoDoc();
		
		// Gera Array de dados para exportação
		$this->exportarTipoDoc();
		
		return $this->dados;
	}
	
	/**
	 * Faz o download do arquivo com os dados gerados da exportação
	 * @param array $dados
	 * @return jsonfile
	 */
	public function download(Array $dados)
	{
		// Retorna arquivo para exportação
	    return Simec_Exportador_ArquivoJson::exportarArquivo($dados, "wf_" . $dados['tipodocumento']['tpddsc']);
	}
	
	/**
	 * Importa arquivo JSON para banco de dados.
	 * @param array $arquivo
	 * @param string $sisid
	 */
	public function importar($arquivo, $sisid)
	{
	    $this->dados = Simec_Exportador_ArquivoJson::importarArquivo($arquivo);
			
		if($this->dados['tipodocumento']['sisid'] != $sisid) {
			throw new \Exception("Erro! Este workflow não pertence a este modulo");		
		}
		
		$this->importarTipoDoc();
	}
	
	/**
	 * Método que gera hash identificador para o tipodocumento, se ainda não tiver.
	 */
	private function gerarHashTipoDoc()
	{
		//consulta se o tipo documento já não possui um hash
		$sql = "SELECT tpdid FROM workflow.tipodocumento WHERE (tpdhash IS NULL or tpdhash = '') AND tpdid = {$this->id}";
	
		$tipodocumento = $this->carregar($sql);
	
		if(!empty($tipodocumento) && is_array($tipodocumento)){
	
			foreach ($tipodocumento as $fluxo) {
			
				$novoHash = $this->criarHash();
			
				$sql = "UPDATE workflow.tipodocumento SET tpdhash = '{$novoHash}' WHERE tpdid = " . $fluxo['tpdid'];
	
				$this->executar($sql);
				$this->commit();
			}
		}
		
		$this->gerarHashEstadoDoc();
	}
	
	/**
	 * Gera hash identificador para o workflow.estadodocumento, se ainda não
	 * tiver.
	 */
	private function gerarHashEstadoDoc()
	{
		//consulta se o tipo documento já não possui um hash
		$sql = "SELECT esdid FROM workflow.estadodocumento WHERE esdhash IS NULL AND tpdid = {$this->id}";
	
		$estados = $this->carregar($sql);
	
		if(!empty($estados) && is_array($estados)){
	
			foreach ($estados  as $estado) {
			
				$novoHash = $this->criarHash();
	
				$sql = "UPDATE workflow.estadodocumento SET esdhash = '{$novoHash}' WHERE esdid = " . $estado['esdid'];
	
				$this->executar($sql);
				$this->commit();
			}
		}
		
		$this->gerarHashAcao();
	}
	
	/**
	 * Gera hash indentificador para cada elemento da workflow.acaoestadodoc, se ainda
	 * não tiver.
	 */
	private function gerarHashAcao()
	{
		$sql = "SELECT 
					acao.aedid
				FROM 
					workflow.tipodocumento AS fluxo
				JOIN
					workflow.estadodocumento AS estado ON estado.tpdid = fluxo.tpdid
				JOIN
					workflow.acaoestadodoc AS acao ON estado.esdid = acao.esdidorigem
				WHERE 
					fluxo.tpdid = {$this->id} AND (acao.aedhash IS NULL OR acao.aedhash = '')";
	
		$acoes = $this->carregar($sql);
	
		if(!empty($acoes) && is_array($acoes)){
	
			foreach($acoes as $acao){
				
				$novoHash = $this->criarHash();
				
				$sql = "UPDATE workflow.acaoestadodoc SET aedhash = '{$novoHash}' WHERE aedid = " . $acao['aedid'];
			
				$this->executar($sql);
				$this->commit();
			}
	
		}
	}
	
	/**
	 * Método que prepara os dados do workflow/ fluxo/ tipo documento para
	 * exportação.
	 */
	private function exportarTipoDoc()
	{
		$sql = "SELECT * FROM workflow.tipodocumento WHERE tpdid = " . $this->id;
	
		$tipodocumento = $this->carregar($sql);
	
		$this->dados['tipodocumento'] = $tipodocumento[0];
		
		$this->exportarEstados();
	}
	
	/**
	 * Método que prepara os dados dos estados do workflow documento para
	 * exportação.
	 */
	private function exportarEstados()
	{
		$sql = "SELECT * FROM workflow.estadodocumento WHERE tpdid = " . $this->id . "ORDER BY esdordem ASC, esdid ASC";
	
		$estados = $this->carregar($sql);
	
		$this->dados['estados'] = $estados;
		
		$this->exportarAcoes();
	}
	
	/**
	 * Prepara os dados das ações para exportação.
	 */
	private function exportarAcoes()
	{
//		$sql = "SELECT
//					acao.*,
//					estado.hashid AS origem,
//					(
//						SELECT
//							destino.hashid
//						FROM
//							workflow.estadodocumento destino
//						WHERE
//							destino.esdid = acao.esdiddestino
//					) AS destino
//				FROM
//					workflow.tipodocumento AS fluxo JOIN workflow.estadodocumento AS estado ON
//					estado.tpdid = fluxo.tpdid JOIN workflow.acaoestadodoc AS acao ON
//					estado.esdid = acao.esdidorigem
//				WHERE
//					fluxo.tpdid = {$this->id}
//				ORDER BY
//					acao.aedordem, acao.aedid";

		$sql = <<<SQL
            SELECT
                acao.*,
                estado.esdhash AS origem,
                esdd.esdhash AS destino
            FROM workflow.tipodocumento AS fluxo
            INNER JOIN workflow.estadodocumento AS estado ON estado.tpdid = fluxo.tpdid
            INNER JOIN workflow.acaoestadodoc AS acao ON estado.esdid = acao.esdidorigem
            INNER JOIN workflow.estadodocumento AS esdd ON esdd.esdid = acao.esdiddestino
            WHERE fluxo.tpdid = {$this->id}
            ORDER BY acao.aedordem, acao.aedid
SQL;


		$acoes = $this->carregar($sql);
		
		$this->dados['acoes'] = $acoes;
		
		$this->exportarPerfis($acoes);
	}
	
	/**
	 * Prepara os dados de perfis para exportação. Utiliza os dados que serão exportados
	 * das ações para obter uma lista de perfis que é salva como um array indexado
	 * pelo hashid de cada ação.
	 *
	 * @param array $acoes Array contendo as informações das ações para exportação
	 * @return array Array contendo os IDs dos perfis agrupados pelo hashid de cada ação
	 */
	private function exportarPerfis($acoes)
	{
		if( !empty($acoes) && is_array($acoes) )
		{
			$dados = array();
	
			foreach($acoes as $acao){
	
	
//				$sql = "SELECT * FROM workflow.estadodocumentoperfil WHERE aedid = " . $acao['aedid'];
                $sql = <<<SQL
                    SELECT
                      pfl.pflcod,
                      pfl.pfldsc
                    FROM workflow.estadodocumentoperfil esp
                    INNER JOIN seguranca.perfil pfl USING(pflcod)
                    WHERE aedid = {$acao['aedid']}
SQL;

	
				$perfis = $this->carregar($sql);
	
				if(!empty($perfis)) foreach($perfis as $perfil){
					 
					//agrupa os perfis em um array indexado pelo hashid da ação correspondente
					$dados[$acao['aedhash']][] = [
					    'pflcod' => $perfil["pflcod"],
                        'pfldsc' => $perfil["pfldsc"]
                    ];
				}
			}
	
			$this->dados['perfis'] = $dados;
		}
	}
	
	/**
	 * Faz a importação dos dados do arquivo para tabela workflow.tipodocumento
	 * @throws \Exception
	 */
	private function importarTipoDoc()
	{	
		$sql = "SELECT tpdid FROM workflow.tipodocumento WHERE tpdhash = '".$this->dados['tipodocumento']['tpdhash']."'";
		
		$tipodocumento = $this->carregar($sql);
		
		if(empty($tipodocumento) || !is_array($tipodocumento)) {
			
			$tpdid = $this->dbinsert($this->dados['tipodocumento'], "workflow.tipodocumento");

			if(count($this->dados['estados']) > 0 && is_array($this->dados['estados'])) {
				
				$this->importarEstadoDoc($tpdid['tpdid']);
				
			}
			
		} else {
			
			//UPDATE
			$this->updateTipoDoc();
				
		}
	}
	
	/**
	 * Faz a importação dos dados do arquivo para tabela workflow.estadodocumento
	 * @param int $tpdid
	 */
	private function importarEstadoDoc($tpdid)
	{
		$estadosid = array();
		
		foreach ($this->dados['estados'] as $key => $values) {
			
			$values['tpdid'] = $tpdid;

			$id = $this->dbinsert($values, "workflow.estadodocumento");
			
			$estadosid[] = array(
					"id" => $id['esdid'],
					"esdhash" => $values['esdhash']
			);
			
            $this->log->addMensagem(sprintf(self::LOG_ESTADO_CRIADO, ($values['esddsc'])));
		}
		
		if(count($this->dados['acoes']) > 0 && is_array($this->dados['acoes'])) {
			
			$this->importarAcaoDoc($estadosid);
					
		}
	}
	
	/**
	 * Faz a importação dos dados do arquivo para tabela workflow.acaoestadodoc
	 * @param int $estadosid
	 */
	private function importarAcaoDoc($estadosid)
	{
		$acaodocid = array();
		
		foreach ($this->dados['acoes'] as $key => $values) {
			
			foreach ($estadosid as $values2) {
				
				if($this->dados['acoes'][$key]['origem'] == $values2['esdhash']) {
					$this->dados['acoes'][$key]['esdidorigem'] = $values2['id'];
				}
				
				if($this->dados['acoes'][$key]['destino'] == $values2['esdhash']) {
					$this->dados['acoes'][$key]['esdiddestino'] = $values2['id'];
				}
				
			}
			
		}
		
		foreach ($this->dados['acoes'] as $values) {
			
			unset($values['origem']);
			unset($values['destino']);
			
			if($values['aeddatainicio'] == '') {
				
				unset($values['aeddatainicio']);
				
			}
			
			if($values['aeddatafim'] == '') {
				
				unset($values['aeddatafim']);
				
			}
			
			unset($values['aedordem']);
			
			$id = $this->dbinsert($values, "workflow.acaoestadodoc");
			
			$acaodocid[] = array(
					"id" => $id['aedid'],
					"aedhash" => $values['aedhash'],
					"aeddscrealizar" => $values['aeddscrealizar']
			);
			
			$this->log->addMensagem(sprintf(self::LOG_ACAO_CRIADO, ($values['aeddscrealizar'])));
		}
		
		if(count($this->dados['perfis']) > 0 && is_array($this->dados['perfis'])) {
			
			$this->importarPerfilAcaoDoc($acaodocid);
		
		}
		
	}
	
	/**
	 * Faz a importação dos dados do arquivo para workflow.estadodocumentoperfil
	 * @param int $acaodocid
	 */
	private function importarPerfilAcaoDoc($acaodocid)
	{
		$perfis = array();
		
		foreach ($this->dados['perfis'] as $key => $values) {
			
			foreach ($acaodocid as $values2) {
				
				if($key == $values2['aedhash']) {
					
					$perfis[] = array(
							"aedid" => $values2['id'],
							"aeddscrealizar" => $values2['aeddscrealizar'],
							"perfil" => $values
					);
					
				}
				
			}
			
		}

		foreach ($perfis as $key => $values) {
			
			foreach ($values['perfil'] as $values2) {

			    $sql = "select pflstatus from seguranca.perfil where pflcod = {$values2['pflcod']}";
			    $pflstatus = $this->carregar($sql)[0];

			    if(!empty($pflstatus)) {
			        if($pflstatus == 'A') {
                        $sql = "INSERT INTO workflow.estadodocumentoperfil (aedid, pflcod) VALUES ('" . $values['aedid'] . "', '" . $values2['pflcod'] . "')";
                        $this->executar($sql);
                        $this->commit();

                        $sqlPerfil = "SELECT pfldsc FROM seguranca.perfil WHERE pflcod = '{$values2['pflcod']}'";
                        $perfil = $this->carregar($sqlPerfil);

                        $this->log->addMensagem(sprintf(self::LOG_ACAO_PERFIL, ($values['aeddscrealizar']), $values2['pfldsc']));
                    } else {
                        $this->log->addMensagem("[Perfil] \"<p>{$values2['pfldsc']}</p>\" está inativo", Simec_Exportador_Log::WARNING);
                    }
                } else {
                    $this->log->addMensagem("[Perfil] \"<p>{$values2['pfldsc']}</p>\" não existe no sistema", Simec_Exportador_Log::WARNING);
                }
			}
			
		}
		
	}
	
	/**
	 * Atualiza workflow.tipodocumento já existente com dados do arquivo de importação
	 */
	private function updateTipoDoc()
	{
		$this->dbupdate($this->dados['tipodocumento'], "workflow.tipodocumento");
		
		//LOG
		$this->updateEstadoDoc();
	}
	
	/**
	 * Atualiza workflow.estadodocumento já existente com dados do arquivo de importação
	 */
	private function updateEstadoDoc()
	{	
		$estadosid = array();
		
		foreach ($this->dados['estados'] as $values) {
			
			$sql = "SELECT esdstatus from workflow.estadodocumento WHERE esdhash = '{$values['esdhash']}'";
			
			$status = $this->carregar($sql);
			
			if(!empty($status) || is_array($status)) {
				
				if($status[0]['esdstatus'] != $values['esdstatus']) {
					
					unset($values['esdid']);
					
					$this->dbupdate($values, 'workflow.estadodocumento');
					
					// LOG [ESTADO] DELETADO
                    $this->log->addMensagem(sprintf(self::LOG_ESTADO_DELETADO, ($values['esddsc'])));
				
				} else {
					
					unset($values['esdid']);
					
					$this->dbupdate($values, 'workflow.estadodocumento');
					
					// LOG [ESTADO] ALTERADO
                    $this->log->addMensagem(sprintf(self::LOG_ESTADO_ALTERADO, ($values['esddsc']), 'Ordem', $values['esdordem']));
					
				}
					
			} else {
				
				// INSERT
				
				$id = $this->dbinsert($values, 'workflow.estadodocumento');
				
				$estadosid[] = array(
						"id" => $id['esdid'],
						"esdhash" => $values['esdhash']
				);
				
				// LOG [ESTADO] INCLUIDO
                $this->log->addMensagem(sprintf(self::LOG_ESTADO_CRIADO, ($values['esddsc'])));
				
			}
			
		}
		
		$this->updateAcaoDoc($estadosid);
	}
	
	/**
	 * Atualiza workflow.acaoestadodoc já existente com dados do arquivo de importação
	 */
	private function updateAcaoDoc($estadosid)
	{		
		$acaodocid = array();
		
		foreach ($this->dados['acoes'] as $values) {
			
			// ESTADO DE ORIGEM/DESTINO P/ LOG
			$origem = '';
			$destino = '';
			foreach ($this->dados['estados'] as $v) {
				if($v['esdid'] == $values['esdidorigem']) {
					$origem = ($v['esddsc']);
				}
				if($v['esdid'] == $values['esdiddestino']) {
					$destino = ($v['esddsc']);
				}
			}
			// FIM
			
			$sql = "SELECT aedstatus FROM workflow.acaoestadodoc WHERE aedhash = '{$values['aedhash']}'";
			$status = $this->carregar($sql);
				
			if(!empty($status) || is_array($status)) {
				
				$sqlOrigem  = "SELECT esdid FROM workflow.estadodocumento WHERE esdhash = '{$values['origem']}'";
				$sqlDestino = "SELECT esdid FROM workflow.estadodocumento WHERE esdhash = '{$values['destino']}'";
				
				$origemid	= $this->carregar($sqlOrigem);
				$destinoid	= $this->carregar($sqlDestino);
				
				$values['esdidorigem']  = $origemid[0]['esdid'];
				$values['esdiddestino'] = $destinoid[0]['esdid'];
				
				if($status[0]['aedstatus'] != $values['aedstatus']) {
					
					unset($values['aedid']);
					unset($values['origem']);
					unset($values['destino']);
					
					if($values['aeddatainicio'] == '') {
						unset($values['aeddatainicio']);
					}
					if($values['aeddatafim'] == '') {
						unset($values['aeddatafim']);
					}
					
					unset($values['aedordem']);
					
					$this->dbupdate($values, 'workflow.acaoestadodoc');
					
					// LOG [AÇÃO] DELETADA
                    $this->log->addMensagem(sprintf(self::LOG_ACAO_DELETADA, ($values['aeddscrealizar'])));
				
				} else {
					
					unset($values['aedid']);
					unset($values['origem']);
					unset($values['destino']);
					
					if($values['aeddatainicio'] == '') {
						unset($values['aeddatainicio']);
					}
					if($values['aeddatafim'] == '') {
						unset($values['aeddatafim']);
					}
					
					unset($values['aedordem']);
					
					$this->dbupdate($values, 'workflow.acaoestadodoc');
					
					// LOG [ESTADO] ALTERADA
					$this->log
                        ->addMensagem(sprintf(self::LOG_ACAO_ALTERADA, ($values['aeddscrealizar']), 'Estado de origem', $origem))
                        ->addMensagem(sprintf(self::LOG_ACAO_ALTERADA, ($values['aeddscrealizar']), 'Estado de destino', $destino))
                        ->addMensagem(sprintf(self::LOG_ACAO_ALTERADA, ($values['aeddscrealizar']), 'Dsc. Realizada', ($values['aeddscrealizada'])))
                        ->addMensagem(sprintf(self::LOG_ACAO_ALTERADA, ($values['aeddscrealizar']), 'Condiçao', $values['aedcondicao']))
                        ->addMensagem(sprintf(self::LOG_ACAO_ALTERADA, ($values['aeddscrealizar']), 'Pós Ação', $values['aedposacao']))
                        ->addMensagem(sprintf(self::LOG_ACAO_ALTERADA, ($values['aeddscrealizar']), 'Pré Ação', $values['aedpreacao']));
				}
					
			} else {
				
				// INSERT
				
				foreach ($this->dados['acoes'] as $key => $values2) {
					foreach ($estadosid as $values3) {
						if($this->dados['acoes'][$key]['origem'] == $values3['esdhash']) {
							$values['esdidorigem'] = $values3['id'];
						}
						if($this->dados['acoes'][$key]['destino'] == $values3['esdhash']) {
							$values['esdiddestino'] = $values3['id'];
						}
					}
				}
				
				unset($values['origem']);
					unset($values['destino']);
					
					if($values['aeddatainicio'] == '') {
						unset($values['aeddatainicio']);
					}
					if($values['aeddatafim'] == '') {
						unset($values['aeddatafim']);
					}
					
					unset($values['aedordem']);
				
				$id = $this->dbinsert($values, 'workflow.acaoestadodoc');
				
				// LOG [AÇÃO] INCLUIDA
                $this->log->addMensagem(sprintf(self::LOG_ACAO_CRIADO, ($values['aeddscrealizar'])));
				
			}
			
		}
		
		$this->updatePerfilAcaoDoc();
	}
	
	/**
	 * Atualiza workflow.estadodocumentoperfil já existente com dados do arquivo de importação
	 */
	private function updatePerfilAcaoDoc ()
	{
		
		$idacoes = array();
		
		foreach ($this->dados['acoes'] as $values) {
			
			$sql = "SELECT aedid, aeddscrealizar FROM workflow.acaoestadodoc WHERE aedhash = '{$values['aedhash']}'";
			$acoes = $this->carregar($sql);
			$idacoes[] = array(
					"id" => $acoes[0]['aedid'],
					"aedhash" => $values['aedhash'],
					"aeddscrealizar" => $acoes[0]['aeddscrealizar']
			);
			
		}
		
		foreach ($this->dados['perfis'] as $key => $values) {
			foreach ($idacoes as $values2) {
				if($key == $values2['aedhash']) {
					$sql = "DELETE FROM workflow.estadodocumentoperfil where aedid = '{$values2['id']}'";
					$this->executar($sql);
					$this->commit();
//				}
//			}
//
//			foreach ($idacoes as $values2) {
//				if($key == $values2['aedhash']) {
					foreach ($values as $values3) {
						
						$sqlPerfil = "SELECT pflstatus FROM seguranca.perfil WHERE pflcod = '{$values3['pflcod']}'";
						$perfil = $this->carregar($sqlPerfil)[0]['pflstatus'];

						if(!empty($perfil)) {
						    if($perfil == 'A') {
                                $sql = "INSERT INTO workflow.estadodocumentoperfil (aedid, pflcod) VALUES ('" . $values2['id'] . "', '" . $values3['pflcod'] . "')";
                                $this->executar($sql);
                                $this->commit();
                                // LOG [AÇÃO] PERFIL
                                $this->log->addMensagem(sprintf(self::LOG_ACAO_PERFIL, $values2['aeddscrealizar'], $values3['pfldsc']));
                            } else {
                                $this->log->addMensagem("[Perfil] <b>\"{$values3['pfldsc']}\"</b> está inativo", Simec_Exportador_Log::WARNING);
                            }
                        } else {
                            $this->log->addMensagem("[Perfil] <b>\"{$values3['pfldsc']}\"</b> não existe no sistema", Simec_Exportador_Log::WARNING);
                        }
						
					}
				}
			}
			
		}
	}
}