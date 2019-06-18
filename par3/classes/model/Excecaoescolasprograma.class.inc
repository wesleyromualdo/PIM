<?php
/**
 * Classe de mapeamento da entidade par3.excecaoescolasprograma.
 *
 * @version $Id$
 * @since 2016.10.31
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Excecaoescolasprograma: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Excecaoescolasprograma($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Excecaoescolasprograma($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $prgid ID do programa
 * @property int $inuid Instrumento unidade (localidade)
 * @property string $eepstatus Status da solicitacao  A = ATIVO I = INATIVO - default: 'A'::bpchar
 * @property string $eepjustificativa Justificativa da solicitação
 * @property string $usucpf CPF de quem executa a ação
 * @property int $eepid ID da tabela - default: nextval('par3.excecaoescolasprograma_eepid_seq'::regclass)
 */
class Par3_Model_Excecaoescolasprograma extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.excecaoescolasprograma';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'eepid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'prgid' => array('tabela' => 'par3.programa', 'pk' => 'prgid'),
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'prgid' => null,
        'inuid' => null,
        'adpid' => null,
        'eepstatus' => null,
        'eepjustificativa' => null,
        'usucpf' => null,
        'eepid' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'prgid' => array('allowEmpty' => true, 'Digits'),
            'inuid' => array('allowEmpty' => true, 'Digits'),
            'eepstatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'eepjustificativa' => array('allowEmpty' => true),
            'usucpf' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))),
            'eepid' => array('Digits'),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    /**
     *
     */
    public function retornaListaSolicitacoes($dados)
    {
        $perfis = pegaPerfilGeral($_SESSION['usucpf']);
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($dados['inuid']);
        $dadosUnidade = $modelInstrumentoUnidade->getDadosUnidade($dados['inuid']);
        //print_r($dadosUnidade);die();
        $inuid = $dados['inuid'];
        $adpid = $dados['adpid'];
        $itrid = $dadosUnidade['itrid'];
        $prgid = $dados['prgid'];
        $validado = $dados['validado'] ? $dados['validado'] : 'IS NULL';

        $modelProadesao = new Par3_Model_Proadesao();
        $proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($prgid, $_SESSION['par3']['adpano_ciclo']);

        $bloqueiaSave = false;

//     	if((strtotime($_SESSION['par3']['mais_escolas']['vigencia'][$_SESSION['par3']['adpano_ciclo']]. ' 12:00:00') < strtotime(date('Y-m-d G:i:s'))))
//     	{
//     		$bloqueiaSave = true;
//     		$disabledCarga = 'disabled ';
//     	}


        $sql = "SELECT 
    				eepid as id_solicitacao,
    				usucpf || ' - ' ||(select usunome from seguranca.usuario  u where u.usucpf = eep.usucpf) as cpf_nome_solicitante,
    				to_char(eepdtinsercao, 'DD/MM/YYYY HH24:MI:SS') as data_insercao,
    				eepjustificativa as justificativa,
    				eepjustificativanegacao as justificativa_negacao,
    				eepdecisao as validacao
    			FROM 
    	    		par3.excecaoescolasprograma eep
    			WHERE
    				adpid = $adpid AND
        			prgid = {$prgid} AND
                    eepdecisao $validado";

        $resultado = $this->carregar($sql);
        $resultado = (is_array($resultado)) ? $resultado : array();

        if (count($resultado) > 0) {
            foreach ($resultado as $k => $v) {
                $retorno[$k]['id_solicitacao'] = $v['id_solicitacao'];
                $retorno[$k]['cpf_nome_solicitante'] = $v['cpf_nome_solicitante'];
                $retorno[$k]['data_insercao'] = $v['data_insercao'];
                $retorno[$k]['justificativa'] = $v['justificativa'];


                if ($v['validacao'] == 'F') {
                    if ((in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)) && (!$bloqueiaSave)) {
                        $retorno[$k]['validacao'] =
                            "
	    					<div id=\"botao_aceita_{$v['id_solicitacao']}\" >
	    						<button onclick=\"aceitaExcecao('{$v['id_solicitacao']}')\" type=\"button\" class=\"btn btn-success salvar\"> Aceitar solicitação </button>
	    					</div>
	    					<div id=\"sucesso_aceita_{$v['id_solicitacao']}\" style='display:none' >
	    						<h2 class=\"glyphicon glyphicon-ok-circle success\" aria-hidden=\"true\">Solicitação aceita</h2>
	    					</div>
	    					<br>
	    					<div id=\"botao_nega_{$v['id_solicitacao']}\" >
	    						<button onclick=\"negaExcecao('{$v['id_solicitacao']}')\" type=\"button\" class=\"btn btn-danger\"> Negar solicitação </button>
	    					</div>
	    					<div id=\"justifica_negacao_{$v['id_solicitacao']}\" style='display:none' >
	    						Justificativa:
	    						<textarea cols=\"30\" rows=\"8\" id=\"justifica_negacao_textarea_{$v['id_solicitacao']}\" name=\"justifica_negacao_{$v['id_solicitacao']}\" ></textarea>
	    						<div id=\"botao_nega_{$v['id_solicitacao']}\" >
	    							<button onclick=\"confirmaExcecao('{$v['id_solicitacao']}')\" type=\"button\"  class=\"btn btn-info\" style=\"background-color:#449D44;\">Confirma</button>
	    							<button onclick=\"cancelaExcecao('{$v['id_solicitacao']}')\" type=\"button\"  class=\"btn btn-danger\" >Cancelar</button>
	    						</div>
	    					</div>
	    					<div id=\"sucesso_negado_{$v['id_solicitacao']}\" style='display:none' >
	    						<h2 class=\"glyphicon glyphicon-remove-sign danger\" aria-hidden=\"true\">Solicitação não aceita</h2>
	    						<p id=\"just_{$v['id_solicitacao']}\" style=\"text-align: justify;\" >
										
	    						 </p>
	    					</div>
	    					
	    					
	    				";
                    } else {
                        $retorno[$k]['validacao'] = "Aguardando avaliação de Solicitação";
                    }
                } else if ($v['validacao'] == 'S') {
                    $retorno[$k]['validacao'] = '<h2 class="glyphicon glyphicon-ok-circle success" aria-hidden="true"> Solicitação aceita</h2>';
                } else if ($v['validacao'] == 'N') {
                    $retorno[$k]['validacao'] = "<h2 class=\"glyphicon glyphicon-remove-sign danger\" aria-hidden=\"true\">Solicitação não aceita</h2>
							    						<p style=\"text-align: justify;\" >					
							    						<b>Justificativa</b>: {$v['justificativa_negacao']}
	    												</p>
	    						";
                }
                $idSolicitacao = $v['id_solicitacao'];
                if ($prgid == PRGID_ABCMAIS) {
                    $sqlEscolas = "
	    			SELECT
						exp.codinep as cod_inep,
						eam.eamnome as nome_escola,
						expcargahoraria as carga_horaria,
						expgrupo as grupo
						 FROM  par3.escolasexcecaoprograma exp
					INNER JOIN par3.escolasabcmais2015 eam ON eam.eamcod = exp.codinep
					WHERE eepid = {$idSolicitacao} AND eam.aemano_ciclo = {$_SESSION['par3']['adpano_ciclo']}";
                } else if ($prgid == PRGID_PROEMI) {
                    $sqlEscolas = "
	    			SELECT
						exp.codinep as cod_inep,
						epe.epenome as nome_escola,
						expcargahoraria as carga_horaria,
						expgrupo as grupo
						FROM  par3.escolasexcecaoprograma exp
					INNER JOIN par3.escolasproemi2015 epe ON epe.epecod = exp.codinep 
					WHERE eepid = {$idSolicitacao}";
                }else if ($prgid == PRGID_MAIS_ALFABETIZACAO) {
                    $sqlEscolas = "
	    			SELECT
						exp.codinep as cod_inep,
						ema.emacodinep as nome_escola,
						expcargahoraria as carga_horaria,
						expgrupo as grupo
						FROM  par3.escolasexcecaoprograma exp
					INNER JOIN par3.escolasmaisalfabetizacao ema ON ema.emacodinep = exp.codinep::INTEGER
					WHERE eepid = {$idSolicitacao}";
                }else if ($prgid == PRGID_ESCOLA_ACESSIVEL) {
                    $sqlEscolas = "
	    			SELECT
						exp.codinep as cod_inep,
						ema.eeacodinep as nome_escola,
						expcargahoraria as carga_horaria,
						expgrupo as grupo
						FROM  par3.escolasexcecaoprograma exp
					INNER JOIN par3.escolas_ea ema ON ema.eeacodinep = exp.codinep::INTEGER
					WHERE eepid = {$idSolicitacao}";
                }
                
                $resultadoEscolas = $this->carregar($sqlEscolas);
                $resultadoEscolas = (is_array($resultadoEscolas)) ? $resultadoEscolas : array();

                $table = "";

                if (count($resultadoEscolas) > 0) {

                    $table = "<table border=1>
								<thead>
								<tr>
									<td>
										Código INEP								
									</td>
									<td>
										Nome da Escola
									</td>
									<td>
										Grupo
									</td>
									<td>
										Carga Horária
									</td>
									
								</tr>
								</thead>
								<tbody>
								";
                    foreach ($resultadoEscolas as $kE => $vE) {
                        $table .= "
		    					<tr>
									<td>
										{$vE['cod_inep']}								
									</td>
									<td>
										{$vE['nome_escola']}
									</td>
									<td>
										{$vE['grupo']}
									</td>
									<td>
										{$vE['carga_horaria']}
									</td>
									
								</tr>
		    					";
                    }
                    $table .= "
		    			</tbody>
		    		</table>
		    		";

                }
                $retorno[$k]['escolas'] = $table;
            }
            return $retorno;
        } else {
            return array();
        }
    }

    /**
     *
     */
    public function alteraStatusSolicitacaoExcecao($dados)
    {

        $justificativa = ($dados['justificativa']) ? $dados['justificativa'] : "''";
        $decisao = $dados['decisao'];
        $eepid = $dados['eepid'];

        if ($eepid && $decisao) {
            $sql = "UPDATE par3.excecaoescolasprograma SET 
                        eepdecisao = '{$decisao}', 
                        eepjustificativanegacao = '{$justificativa}'  
                    WHERE 
                        eepid = {$eepid}";

            $this->executar($sql);
            return $this->commit();
        } else {
            return false;
        }
    }

    /**
     *
     */
    public function possuiSolicitacaoPorGrupo($arrParamExcecao)
    {
        $perfis = pegaPerfilGeral($_SESSION['usucpf']);

        $inuid = $arrParamExcecao['inuid'];
        $adpid = $arrParamExcecao['adpid'];
        $grupo = $arrParamExcecao['grupo'];
        $prgid = $arrParamExcecao['prgid'];

        if (in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)) {

            if ($grupo && $inuid && $adpid && $prgid) {

                $sql = "SELECT eep.eepid FROM par3.excecaoescolasprograma eep
						INNER JOIN par3.escolasexcecaoprograma exp ON exp.eepid = eep.eepid and expstatus = 'A'
					    WHERE 
                            inuid = {$inuid} AND
                            adpid = $adpid AND
                            expgrupo = {$grupo} AND 
                            prgid = {$prgid}";

                return ($this->pegaUm($sql)) ? true : false;
            } else {
                return false;
            }

        } else {
            return false;
        }

    }

    /**
     *
     */
    public function salvaFormulario($dados)
    {
        $usucpf = (string)$_SESSION['usucpf'];
        $prgid = $dados['prgid'];
        $inuid = $dados['inuid'];
        $adpid = $dados['adpid'];
        $eepjustificativa = $dados['justificativa'];

        $sqlInsert1 = " INSERT INTO par3.excecaoescolasprograma (usucpf, prgid, inuid, adpid, eepjustificativa)
                        VALUES ('{$usucpf}', {$prgid}, {$inuid}, $adpid, '{$eepjustificativa}')
        	            RETURNING eepid;";

        $idAtual = $this->pegaUm($sqlInsert1);

//         $arrEscolas = $dados['cod_inep'];
        $arrEscolas = $dados['escola_checada'];

        if ($idAtual) {
            $iCarga = 0;
            if (is_array($arrEscolas)) {
                foreach ($arrEscolas as $k => $codInep) {
                    if ($codInep) {
                        $arrInsert[$k]['codinep'] = $codInep;
                        $arrInsert[$k]['eepid'] = $idAtual;

// 	        			$arrInsert[$k]['exppossuicargahoraria'] = ($dados['arrPossuiCargaHoraria'][$k] == 's') ? 'TRUE': 'FALSE';
                        $arrInsert[$k]['exppossuicargahoraria'] = ($dados['carga_horaria_' . $codInep] != '') ? 'TRUE' : 'FALSE';
                        if ($dados['carga_horaria_' . $codInep] != '') {
                            if ($prgid == 8) {
// 	        				    $arrInsert[$k]['expcargahoraria'] = $dados['aeacargahoraria'][$iCarga];
                                $arrInsert[$k]['expcargahoraria'] = $dados['carga_horaria_' . $codInep];
                            } else {
                                $arrInsert[$k]['expcargahoraria'] = $dados['carga_horaria_' . $codInep];
                            }
                            $iCarga++;
                        } else {
                            $arrInsert[$k]['expcargahoraria'] = 'NULL';
                        }
                        $arrInsert[$k]['expgrupo'] = $dados['grupo_escola'][$codInep];
                    }
                }
            }
            $baseSqlEscolas =
                "INSERT INTO par3.escolasexcecaoprograma
		        	(codinep, eepid, exppossuicargahoraria, expcargahoraria, expgrupo)
		        VALUES";
            if (is_array($arrInsert)) {
                foreach ($arrInsert as $k => $v) {
                    $registroEscola[] = "( {$v['codinep']}, {$v['eepid']}, {$v['exppossuicargahoraria']}, {$v['expcargahoraria']}, {$v['expgrupo']} )";
                }

                $escolas = implode(', ', $registroEscola);
                $sqlEscolas = $baseSqlEscolas . $escolas;

            }

            $this->executar($sqlEscolas, false);
            return $this->commit();
        } else {
            return false;
        }


    }

    public function salvaFormularioAlfabetizacao($dados)
    {
        $usucpf = (string)$_SESSION['usucpf'];
        $prgid = $dados['prgid'];
        $inuid = $dados['inuid'];
        $adpid = $dados['adpid'];
        $eepjustificativa = $dados['justificativa'];
        $arrEscolas = $dados['escola_checada'];

        if (!empty($dados['eepid'])) {

            $sql = <<<SQL
    UPDATE par3.excecaoescolasprograma SET usucpf = '{$usucpf}', prgid = {$prgid}, inuid = {$inuid}, adpid = {$adpid}, eepjustificativa = '{$eepjustificativa}'
    WHERE eepid = {$dados['eepid']};
SQL;
            $this->executar($sql);
            $this->commit();

            $idAtual = $dados['eepid'];

        } else {
            $sqlInsert1 = <<<SQL
INSERT INTO par3.excecaoescolasprograma (usucpf, prgid, inuid, adpid, eepjustificativa)
                        VALUES ('{$usucpf}', {$prgid}, {$inuid}, {$adpid}, '{$eepjustificativa}')
        	            RETURNING eepid;
SQL;
            $idAtual = $this->pegaUm($sqlInsert1);
        }

        if ($idAtual && is_array($arrEscolas)) {
            foreach ($arrEscolas as $k => $codInep) {
                if ($codInep) {
                    $sql = <<<SQL
                             SELECT * FROM par3.escolasmaisalfabetizacao WHERE emacodinep = {$codInep}
SQL;
                    $escola = $this->pegaLinha($sql);
                    $arrInsert[$k]['codinep'] = $codInep;
                    $arrInsert[$k]['eepid'] = $idAtual;
                    $arrInsert[$k]['exppossuicargahoraria'] = 'TRUE';
                    $arrInsert[$k]['expcargahoraria'] = $escola['emacargahoraria'];
                    $arrInsert[$k]['expgrupo'] = ($escola['emacargahoraria'] == 10 ? 1 : 2);
                }
            }

            $baseSqlEscolas = " INSERT INTO par3.escolasexcecaoprograma (codinep, eepid, exppossuicargahoraria, expcargahoraria, expgrupo) VALUES ";

            if (is_array($arrInsert)) {
                foreach ($arrInsert as $k => $v) {
                    $registroEscola[] = "( {$v['codinep']}, {$v['eepid']}, {$v['exppossuicargahoraria']}, {$v['expcargahoraria']}, {$v['expgrupo']} )";
                }
                $escolas = implode(',', $registroEscola);
                $sqlEscolas = $baseSqlEscolas . $escolas;
            }

            $this->executar($sqlEscolas, false);
            return $this->commit();

        } else {
            return false;
        }
    }

    public function salvaFormularioEA($dados)
    {
        $usucpf = (string)$_SESSION['usucpf'];
        $prgid = $dados['prgid'];
        $inuid = $dados['inuid'];
        $adpid = $dados['adpid'];
        $eepjustificativa = $dados['justificativa'];
        $arrEscolas = $dados['escola_checada'];

        if (!empty($dados['eepid'])) {

            $sql = <<<SQL
    UPDATE par3.excecaoescolasprograma SET usucpf = '{$usucpf}', prgid = {$prgid}, inuid = {$inuid}, adpid = {$adpid}, eepjustificativa = '{$eepjustificativa}'
    WHERE eepid = {$dados['eepid']};
SQL;
            $this->executar($sql);
            $this->commit();

            $idAtual = $dados['eepid'];

        } else {
            $sqlInsert1 = <<<SQL
INSERT INTO par3.excecaoescolasprograma (usucpf, prgid, inuid, adpid, eepjustificativa)
                        VALUES ('{$usucpf}', {$prgid}, {$inuid}, {$adpid}, '{$eepjustificativa}')
        	            RETURNING eepid;
SQL;
            $idAtual = $this->pegaUm($sqlInsert1);
        }

        if ($idAtual && is_array($arrEscolas)) {
            foreach ($arrEscolas as $k => $codInep) {
                if ($codInep) {
                    $sql = <<<SQL
                             SELECT * FROM par3.escolas_ea WHERE eeacodinep = {$codInep}
SQL;
                    $escola = $this->pegaLinha($sql);
                    $arrInsert[$k]['codinep'] = $codInep;
                    $arrInsert[$k]['eepid'] = $idAtual;
                    $arrInsert[$k]['exppossuicargahoraria'] = 'TRUE';
                    $arrInsert[$k]['expcargahoraria'] = $escola['eeacargahoraria'];
                    $arrInsert[$k]['expgrupo'] = ($escola['eeacargahoraria'] == 10 ? 1 : 2);
                }
            }

            $baseSqlEscolas = " INSERT INTO par3.escolasexcecaoprograma (codinep, eepid, exppossuicargahoraria, expcargahoraria, expgrupo) VALUES ";

            if (is_array($arrInsert)) {
                foreach ($arrInsert as $k => $v) {
                    $registroEscola[] = "( {$v['codinep']}, {$v['eepid']}, {$v['exppossuicargahoraria']}, {$v['expcargahoraria']}, {$v['expgrupo']} )";
                }
                $escolas = implode(',', $registroEscola);
                $sqlEscolas = $baseSqlEscolas . $escolas;
            }

            $this->executar($sqlEscolas, false);
            return $this->commit();

        } else {
            return false;
        }
    }
}
