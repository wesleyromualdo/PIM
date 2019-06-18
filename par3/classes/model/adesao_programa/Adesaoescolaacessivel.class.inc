<?php
/**
 * Classe de mapeamento da entidade par3.adesaoescolaacessivel.
 *
 * @version $Id$
 * @since 2018.04.26
 */

/**
 * Par3_Model_Adesaoescolaacessivel: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Humberto Dias Soares Junior <humberto.junior@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Adesaoescolaacessivel($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Adesaoescolaacessivel($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $adpid 
 * @property \Datetime(Y-m-d H:i:s) $aesdatainclusao  - default: now()
 * @property bool $aesinseridaaposenvio FLAG INSERIDA APÓS O ENVIO - default: false
 * @property string $aesstatus Status (para exclusão lógica) - default: 'A'::bpchar
 * @property string $aescpf CPF de quem inseriu
 * @property string $estuf Código do Estado (SIGLA)
 * @property string $muncod Código do município
 * @property int $inuid ID da entidade ao qual a resposta faz parte
 * @property int $codinep Código do INEP
 * @property int $aesid ID da tabela - default: nextval('par3.adesaoescolaacessivel_aesid_seq'::regclass)
 */
class Par3_Model_Adesaoescolaacessivel extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.adesaoescolaacessivel';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'aesid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
        'adpid' => array('tabela' => 'par3.prodesaoprograma', 'pk' => 'adpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'adpid' => null,
        'aesdatainclusao' => null,
        'aesinseridaaposenvio' => null,
        'aeavalordestinado' => null,
        'aesstatus' => null,
        //'aescargahoraria' => null,
        'aescpf' => null,
        'estuf' => null,
        'muncod' => null,
        'inuid' => null,
        'codinep' => null,
        'aesid' => null,
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
            'adpid' => array('allowEmpty' => true,'Digits'),
            'aesdatainclusao' => array('allowEmpty' => true),
            'aesinseridaaposenvio' => array('allowEmpty' => true),
            'aesstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            //'aescargahoraria' => array('allowEmpty' => true,'Digits'),
            'aescpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'estuf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'muncod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 7))),
            'inuid' => array('allowEmpty' => true,'Digits'),
            'codinep' => array('allowEmpty' => true,'Digits'),
            'aesid' => array('Digits'),
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
    
    
    public function existeEscolas($inuid, $adpid)
    {
        $sql_inst = "SELECT itrid FROM par3.instrumentounidade WHERE inuid = {$inuid}";
        $itrid =  $this->pegaLinha($sql_inst);
        $itrid =  ($itrid ? $itrid['itrid']: '');

        if ($itrid == 2) {
            $arrWhere[] = " eearede = 'Municipal' ";
            $join = " INNER JOIN par3.instrumentounidade iu ON iu.muncod = aea.muncod ";
        } elseif ($itrid == 1 || $itrid == 3) {
            $arrWhere[] = " eearede = 'Estadual' ";
            $join = " INNER JOIN par3.instrumentounidade iu ON iu.estuf = aea.estuf ";
        }

        if ($adpid) {
            $sqlWhere = ' AND ' . implode(' AND ', $arrWhere);            
            $sql = " SELECT count(*) AS qtd_registros FROM par3.escolas_ea aea {$join} WHERE inuid = {$inuid}  {$sqlWhere} ";
            $result = $this->pegaUm($sql);
        }
        return ($result != '') ? true : false;
    }
    
    public function retornaQtdSalvoGrupo($dados)
    {
        $inuid = $dados['inuid'];
        $adpid = $dados['adpid'];
        
        if ($adpid) {
            $sql = "
                    SELECT 
                            count(aesid) 
                    FROM 
                            par3.adesaoescolaacessivel
                    WHERE 
                            inuid = {$inuid} AND
                    adpid = $adpid";

            $resultado = $this->pegaUm($sql);
        }
        return (!$resultado) ? '0' : $resultado;
    }
    
    public function deletaCargaPorGrupo($dados)
    {
        $inuid = $dados['inuid'];
        $adpid = $dados['adpid'];
        $grupo = $dados['grupo'];

        if ($inuid && $grupo) {
            $sql = "
    			UPDATE par3.adesaoescolaacessivel SET 
	    			aescargahoraria = NULL
	    		WHERE
	    			inuid = '{$inuid}' AND
                    adpid = $adpid AND
	    			aesgrupo = '{$grupo}'"; //verificar aes grupo 

            $this->executar($sql);
        }
    }

    
    /*
     * 
     * @return retorna os códigos das escolas que já foram selecionadas na tela que está para abrir
     * */
    public function retornaCodEntidadeMarcados($dados)
    {
        $inuid = $dados['inuid'];
        $adpid = $dados['adpid'];
        
        if ($adpid) {
            $sql = "SELECT 
                            DISTINCT codinep
                    FROM
                            par3.adesaoescolaacessivel
                    WHERE 1 = 1 AND
                    adpid = $adpid AND
                    inuid = {$inuid} ";
                    
            $resultado = $this->carregar($sql);
        }

        $resultado = (is_array($resultado)) ? $resultado : array();

        if (count($resultado) > 0) {
            foreach ($resultado as $k => $v) {
                $arrReturn[] = $v['codinep'];
            }
        }
        
        return $arrReturn;
    }
    
    public function atualizaValorSaldoDisponivel($arrParametros){
        
        $objIstrumentoUnidade = new Par3_Model_InstrumentoUnidade();
        // pegar inuid e verificar se é UF ou Municipio
        $inuid = $arrParametros['inuid'];
        $pfaid = $arrParametros['pfaid'];
        $hsdasaldo = str_replace(',', '.', str_replace('.', '', $arrParametros['valor_saldo_disponivel']));
        $usucpf = $_SESSION['usucpf'];  
        $dadosUnidade = $objIstrumentoUnidade->getDadosUnidade($inuid);
        $itrid = $dadosUnidade['itrid'];
        
        if ($itrid == 2) {
            $estuf = NULL;
            $muncod = $dadosUnidade['muncod'];
            // verifica se o registro de saldo já existe para o Municipio na adesão 
            if ($pfaid && $muncod) {
                $sql = "SELECT hsdaid FROM par3.historicosaldodisponiveladesao
                        WHERE hsdamuncod = '{$muncod}' AND pfaid = {$pfaid} ";

                $resultadoMun = $this->pegaUm($sql);
            }
            // se existir então atualiza
            if ($resultadoMun) {
                $sql = "UPDATE par3.historicosaldodisponiveladesao SET 
	    		hsdasaldo = '{$hsdasaldo}', hsdadata = 'NOW()', usucpf = '{$usucpf}'
	    		WHERE hsdamuncod = '{$muncod}' AND pfaid = {$pfaid} AND hsdaid = {$resultadoMun} ";

                $this->executar($sql);
                $res = $this->commit();
            } else {               
                $sqlInsert = "INSERT INTO par3.historicosaldodisponiveladesao 
                              (hsdaestuf, hsdamuncod, hsdasaldo, hsdadata, hsdastatus, usucpf, pfaid)
                              VALUES
                              (NULL, '{$muncod}', '{$hsdasaldo}', 'NOW()', 'A', '{$usucpf}', $pfaid ) ";
                              
                $this->executar($sqlInsert);
                $res = $this->commit();
            }

        } elseif ($itrid == 1 || $itrid == 3) {
            $estuf = $dadosUnidade['estuf'];
            $muncod = NULL;
            // verifica se o registro de saldo já existe para a UF na adesão 
            if ($pfaid && $estuf) {
                $sql = "SELECT hsdaid FROM par3.historicosaldodisponiveladesao
                        WHERE hsdaestuf = '{$estuf}' AND pfaid = {$pfaid} ";

                $resultadoUf = $this->pegaUm($sql);
            }                     
            // se existir então atualiza   
            if ($resultadoUf) {
                $sql = "UPDATE par3.historicosaldodisponiveladesao SET 
	    		hsdasaldo = '{$hsdasaldo}', hsdadata = 'NOW()', usucpf = '{$usucpf}'
	    		WHERE hsdaestuf = '{$estuf}' AND pfaid = {$pfaid} AND hsdaid = {$resultadoUf} ";

                $this->executar($sql);
                $res = $this->commit();
            } else {
                $sqlInsert = "INSERT INTO par3.historicosaldodisponiveladesao 
                              (hsdaestuf, hsdamuncod, hsdasaldo, hsdadata, hsdastatus, usucpf, pfaid)
                              VALUES
                              ('{$estuf}', NULL, '{$hsdasaldo}', 'NOW()', 'A', '{$usucpf}', $pfaid ) ";
                              
                $this->executar($sqlInsert);
                $res = $this->commit();
            }
        }
        if($res){
            return true;
        }
        
    }
    
    public function salvaFormularioEscolasEA($arrParametros)
    {      
        $resVerificaSaldo = $this->validaSaldoDisponivel($arrParametros); 
        
        if($resVerificaSaldo){
            $arrReturn['retorno'] = 'error';
            $arrReturn['mensagem'] = "Saldo indisponível, não foi possível salvar as escolas selecionadas!";
            return $arrReturn;
        }
        
        $atualizaSaldo = $this->atualizaValorSaldoDisponivel($arrParametros);
        if (!$atualizaSaldo) {
            $arrReturn['retorno'] = 'error';
            $arrReturn['mensagem'] = "Erro ao Salvar";
            return $arrReturn;
        }
        // Objetos utilizados
        $objIstrumentoUnidade = new Par3_Model_InstrumentoUnidade();

        // Dados enviados por parametro
        $inuid = $arrParametros['inuid'];
        $adpid = $arrParametros['adpid'];
        $apta = $arrParametros['apta'];
        $cpf = $_SESSION['usucpf'];        

        $docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma WHERE adpid = $adpid");

        $enviadoApos = FALSE;
        if ($docid) {
            $situacaoAtual = wf_pegarEstadoAtual($docid);
            if (($situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_ESCOLAACESSIVEL)) {
                $enviadoApos = TRUE;
            }
        }

        // Busca os dados da unidade
        $dadosUnidade = $objIstrumentoUnidade->getDadosUnidade($inuid);
        if (!is_array($dadosUnidade)) {
            $arrReturn['retorno'] = 'error';
            $arrReturn['mensagem'] = "Erro ao Salvar";
            return $arrReturn;
        }
        // Dados retornados da unidade
        $itrid = $dadosUnidade['itrid'];
        $estuf = $dadosUnidade['estuf'];
        $muncod = $dadosUnidade['muncod'];

        /**********************************************/

        if ((!$enviadoApos)) {
            $this->executar("DELETE FROM par3.adesaoescolaacessivel WHERE adpid = $adpid AND inuid = {$inuid} ");
        } else {
            if ((!$enviadoApos)) {
                $arrReturn['retorno'] = 'error';
                $arrReturn['mensagem'] = "Erro ao Salvar";
                return $arrReturn;
            }
        }
        /**********************************************/

        $arrEscolas = $arrParametros['escola_checada'];

        if (is_array($arrEscolas)) {
            foreach ($arrEscolas as $k => $codInep) {                                
                $arrDadosSalvar = array();
                
                $dadosPorEscola = $this->pegaLinha("SELECT eeavalordestinado, muncod FROM par3.escolas_ea WHERE eeacodinep = $codInep");
                
                $arrDadosSalvar['codinep'] = $codInep;
                $arrDadosSalvar['inuid'] = $inuid;
                $arrDadosSalvar['adpid'] = $adpid;
                $arrDadosSalvar['estuf'] = $estuf;
                $arrDadosSalvar['muncod'] = $dadosPorEscola['muncod'];
                $arrDadosSalvar['aescpf'] = $cpf;
                $arrDadosSalvar['aesinseridaaposenvio'] = $enviadoApos;
                $arrDadosSalvar['aeavalordestinado'] = $dadosPorEscola['eeavalordestinado'];

//                if ($itrid == 2) {
//                    $arrDadosSalvar['muncod'] = $muncod;
//                }
                $salvo = $this->insereEscolaEA($arrDadosSalvar);

                if ($salvo) {
                    continue;
                } else {
                    $arrReturn['retorno'] = 'error';
                    $arrReturn['mensagem'] = "Erro ao Salvar";
                    return $arrReturn;
                }
            }

            if ($this->commit()) {
                $arrReturn['retorno'] = 'success';
                $arrReturn['mensagem'] = "Formulário de escolas salvo com sucesso";
                return $arrReturn;
            }
        } else {
            $arrReturn['retorno'] = 'error';
            $arrReturn['mensagem'] = "Erro ao Salvar";
            return $arrReturn;
        }
    }
    
    public function validaSaldoDisponivel($arrParametros){
        
        $arrEscolas = $arrParametros['escola_checada'];        
        $vlrDispMun = $arrParametros['valor_disponivel_mun'];
        $vlrDispUf = $arrParametros['valor_disponivel_uf'];
        
        $subTotal = 0;
        $valorDisp = 0;
        
        if ($vlrDispMun && $vlrDispMun > 0){
            $valorDisp = $vlrDispMun;
        } 
        
        if ($vlrDispUf && $vlrDispUf > 0){
            $valorDisp = $vlrDispUf;
        } 
        
        if (is_array($arrEscolas)) {
            foreach ($arrEscolas as $k => $codInep) {
                $dadosPorEscola = $this->pegaLinha("SELECT eeavalordestinado FROM par3.escolas_ea WHERE eeacodinep = $codInep");                
                $subTotal = $subTotal + $dadosPorEscola['eeavalordestinado'];                
            }
            if($valorDisp < $subTotal ){
                return true;
            }
        } else {
            return false;
        }
    }
    
    
    function insereEscolaEA($arrDadosSalvar)
    {
        $codinep = $arrDadosSalvar['codinep'];
        $inuid = $arrDadosSalvar['inuid'];
        $adpid = $arrDadosSalvar['adpid'];
        $estuf = $arrDadosSalvar['estuf'];
        $aescpf = $arrDadosSalvar['aescpf'];
        $aeavalordestinado = $arrDadosSalvar['aeavalordestinado'];
        $muncod = $arrDadosSalvar['muncod'];
        $aesinseridaaposenvio = ($arrDadosSalvar['aesinseridaaposenvio']) ? 'TRUE' : 'FALSE';

        $sqlInsert = "
    		INSERT INTO 
                    par3.adesaoescolaacessivel (codinep, inuid, adpid, muncod, estuf, aescpf, aesinseridaaposenvio, aeavalordestinado )
		VALUES
                    ({$codinep}, {$inuid}, $adpid, {$muncod}, '{$estuf}','{$aescpf}', $aesinseridaaposenvio, $aeavalordestinado )		
    	";
        $this->executar($sqlInsert);

        if ($this->commit()) {
            return true;
        } else {
            return false;
        }
    }
    
    public function deletaGeralPorUnidade($adpid, $prgid)
    {
        if(!empty($adpid) && !empty($prgid))
        {
            $this->executar("DELETE FROM {$this->stNomeTabela} WHERE adpid = {$adpid};");

            $this->commit();
        }
    }
    
    public function insertTotalSaldoNaoUtilizado($inuid, $pfaid, $adpano_ciclo, $itrid)
    {
        if(!empty($inuid) && !empty($pfaid) && !empty($adpano_ciclo) && !empty($itrid) ) {
            
            if ($itrid == 2) {
                $where = " and ea.eearede = 'Municipal' ";
            } elseif ($itrid == 1 || $itrid == 3) {
                $where = " and ea.eearede = 'Estadual' ";
            }
            
            $sql = "select SUM(ea.eeavalordestinado)
                    from par3.prodesaoprograma pro
                    inner join par3.instrumentounidade iu on iu.inuid = pro.inuid
                    inner join par3.escolas_ea ea on ea.muncod = iu.muncod
                    where pro.adpresposta = 'N' 
                    and pro.pfaid = {$pfaid} 
                    and adpano_ciclo = '{$adpano_ciclo}'
                    and ea.eeaapta = true 
                    and ea.eeaselecionada = true 
                    and iu.inuid = {$inuid} $where; ";

            $resultSaldo = $this->pegaUm($sql);
            
            $arrParametros = array();
            
            $arrParametros['inuid'] = $inuid;
            $arrParametros['pfaid'] = $pfaid;
            $arrParametros['valor_saldo_disponivel'] = $resultSaldo;
            
            if ($resultSaldo > 0) {
                $this->atualizaValorSaldoDisponivel($arrParametros);                
            } 
        } 
    }
    
    public function recuperaSaldoNaoUtilizado($inuid, $pfaid, $adpano_ciclo, $itrid)
    {
        if(!empty($inuid) && !empty($pfaid) && !empty($adpano_ciclo) && !empty($itrid) ) {
            
            if ($itrid == 2) {
                $join = " inner join par3.instrumentounidade iu on iu.muncod = hd.hsdamuncod ";
            } elseif ($itrid == 1 || $itrid == 3) {
                $join = " inner join par3.instrumentounidade iu on iu.estuf = hd.hsdaestuf ";
            }
            
            $sql = "select hd.hsdasaldo 
                    from par3.historicosaldodisponiveladesao hd
                    inner join par3.proadesao pa on pa.pfaid = hd.pfaid
                    inner join par3.prodesaoprograma pdp on pdp.pfaid = hd.pfaid
                    inner join par3.adesaoescolaacessivel aes on aes.adpid = pdp.adpid
                    $join
                    where 
                    iu.inuid = {$inuid}
                    and pdp.inuid = {$inuid}
                    and hd.hsdastatus = 'A' 
                    and pa.pfaid = {$pfaid}
                    and pa.pfaano = '{$adpano_ciclo}'
                    and hd.hsdasaldo > 0 ";
            
            $result = $this->pegaUm($sql);
        } else {
            $result = 0;
        }
        return $result;
    }
    
    public function listaAdesoesEA($ano, $pfaid = 12)
    {
        $sql = <<<SQL
                select *	
            from
            (
            select
                distinct 'ESCOLA ACESSÍVEL' as programa,
                inu.estuf as uf,
                inu.muncod as cod_municipio,
                inu.inudescricao as municipio_descricao,
                case
                    when inu.itrid in(
                            1,
                            3
                    ) then 'Estadual'
                    else 'Municipal'
                end as esfera,
                case
                    when adpresposta = 'C' then 'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
                    when adpresposta is null then 'Entrou mas não fez registro (não aceitou, nem negou)'
                    when adpresposta = 'S' then 'Aceitou ao termo'
                    when adpresposta = 'N' then 'Não aceitou o termo'
                end as situacao_aceitacao_termo,
                esd.esddsc as situacao_adesao,
                inu.inuid,
                pro.adpid
            from
                par3.prodesaoprograma pro
            left join workflow.documento doc on
                doc.docid = pro.docid
            left join workflow.estadodocumento esd on
                esd.esdid = doc.esdid
            left join par3.instrumentounidade inu on
                pro.inuid = inu.inuid
            left join par3.adesaoescolaacessivel adm on
                adm.inuid = pro.inuid
            where
                adpano_ciclo = $ano
                and pfaid = $pfaid
	union select
                'ESCOLA ACESSÍVEL' as programa,
                inu.estuf as uf,
                inu.muncod as cod_municipio,
                inu.inudescricao as municipio_descricao,
                case
                        when inu.itrid in(
                                1,
                                3
                        ) then 'Estadual'
                        else 'Municipal'
                end as esfera,
                'Não acessou o programa' as situacao_aceitacao_termo,
                'Não acessou o programa' as situacao_adesao,
                inuid,
                null
            from
                par3.instrumentounidade inu
            where
                inuid not in(
                    select
                        inuid
                    from
                        par3.prodesaoprograma
                    where
                        pfaid = $pfaid
                        and adpano_ciclo = $ano
                )
	) as foo;

SQL;
        $result = $this->carregar($sql);
        $result = (is_array($result)) ? $result : array();
        return  (count($result) > 0) ? $result : false;

    }

    public function getDadosGraficoUfSituacao($ano, $pfaid = 12, $desc){
        return <<<SQL
SELECT
   {$desc}  AS DESCRICAO,
  COUNT(uf) AS VALOR,
  to_json( array_agg( uf ) ) as id

FROM (
       SELECT
         DISTINCT
         inu.estuf  AS uf,
         CASE WHEN adpresposta = 'C'
           THEN
             'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
         WHEN adpresposta IS NULL
           THEN
             'Entrou mas não fez registro (não aceitou, nem negou)'
         WHEN adpresposta = 'S'
           THEN
             'Aceitou ao termo'
         WHEN adpresposta = 'N'
           THEN
             'Não aceitou o termo'
         END        AS situacao_aceitacao_termo,
         
         CASE WHEN adpresposta = 'C'
           THEN
             'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
         WHEN adpresposta IS NULL
           THEN
            'Entrou mas não fez registro (não aceitou, nem negou)'
         WHEN adpresposta = 'S'
           THEN
            'Não aceitou o termo'
         WHEN adpresposta = 'N'
           THEN
            'Aceitou ao termo'
         END        AS id,
         
         esd.esddsc AS situacao_adesao

       FROM par3.prodesaoprograma pro
         LEFT JOIN workflow.documento doc ON doc.docid = pro.docid
         LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
         LEFT JOIN par3.instrumentounidade inu ON pro.inuid = inu.inuid
         LEFT JOIN par3.adesaoescolaacessivel adm ON adm.inuid = pro.inuid
       WHERE
         pro.adpano_ciclo = {$ano}
         AND pro.pfaid = {$pfaid}
         AND inu.itrid IN (1, 3)

      UNION

         SELECT
           DISTINCT estuf,
           'Não acessou o programa' AS situacao_aceitacao_termo,
           'Não acessou o programa' AS situacao_adesao,
           '' as id
         FROM par3.instrumentounidade
        WHERE inuid NOT IN (SELECT inuid FROM par3.prodesaoprograma WHERE pfaid = {$pfaid} AND  adpano_ciclo = {$ano} )
        AND itrid IN (1, 3)
     ) AS foo
  GROUP BY {$desc},id
SQL;
    }
    
    public function getDadosGraficoUfSituacaoAdesao($ano, $pfaid = 12){
        return <<<SQL
        SELECT
            situacao_adesao AS DESCRICAO,
            COUNT(uf) AS VALOR,
            to_json( array_agg( uf ) ) as id
        FROM (
            SELECT
              DISTINCT
              inu.estuf  AS uf,
              CASE WHEN adpresposta = 'C'
                THEN
                  'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
              WHEN adpresposta IS NULL
                THEN
                  'Entrou mas não fez registro (não aceitou, nem negou)'
              WHEN adpresposta = 'S'
                THEN
                  'Aceitou ao termo'
              WHEN adpresposta = 'N'
                THEN
                  'Não aceitou o termo'
              END AS situacao_aceitacao_termo,

              CASE WHEN adpresposta = 'C'
                THEN
                  'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
              WHEN adpresposta IS NULL
                THEN
                 'Entrou mas não fez registro (não aceitou, nem negou)'
              WHEN adpresposta = 'S'
                THEN
                 'Aceitou ao termo'
              WHEN adpresposta = 'N'
                THEN
                 'Não aceitou o termo'
              END AS id,         
              esd.esddsc AS situacao_adesao

            FROM par3.prodesaoprograma pro
              LEFT JOIN workflow.documento doc ON doc.docid = pro.docid
              LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
              LEFT JOIN par3.instrumentounidade inu ON pro.inuid = inu.inuid
              LEFT JOIN par3.adesaoescolaacessivel adm ON adm.inuid = pro.inuid
            WHERE
              pro.adpano_ciclo = {$ano}
              AND pro.pfaid = {$pfaid}
              AND inu.itrid IN (1, 3)
           UNION
              SELECT
                DISTINCT i.estuf,
                'Não acessou o programa' AS situacao_aceitacao_termo,
                'Não acessou o programa' AS situacao_adesao,
                case
                     when i.estuf is not null then 'Não acessou o programa'
                     when i.estuf is not null then 'Não acessou o programa'
                 end as id
              FROM par3.instrumentounidade i
             WHERE inuid NOT IN (SELECT inuid FROM par3.prodesaoprograma WHERE pfaid = {$pfaid} AND  adpano_ciclo = {$ano} )             
             AND i.itrid IN (1, 3)
          ) AS foo
       GROUP BY situacao_adesao, id
SQL;
    }

    public function getDadosGraficoMunicipioSituacao($ano, $pfaid = 12, $desc){
       /* echo <<<SQL
SELECT
  {$desc}  AS DESCRICAO,
  COUNT(muncod) AS VALOR,
  to_json( array_agg( muncod ) ) as id

FROM (
       SELECT
         DISTINCT
         inu.muncod  AS muncod,
         CASE WHEN adpresposta = 'C'
           THEN
             'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
         WHEN adpresposta IS NULL
           THEN
             'Entrou mas não fez registro (não aceitou, nem negou)'
         WHEN adpresposta = 'S'
           THEN
             'Aceitou ao termo'
         WHEN adpresposta = 'N'
           THEN
             'Não aceitou o termo'
         END        AS situacao_aceitacao_termo,
        CASE WHEN adpresposta = 'C'
           THEN
             'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
         WHEN adpresposta IS NULL
           THEN
            'Entrou mas não fez registro (não aceitou, nem negou)'
         WHEN adpresposta = 'S'
           THEN
            'Aceitou ao termo'
         WHEN adpresposta = 'N'
           THEN
            'Não aceitou o termo'
         END        AS id,
         esd.esddsc AS situacao_adesao

       FROM par3.prodesaoprograma pro
         LEFT JOIN workflow.documento doc ON doc.docid = pro.docid
         LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
         LEFT JOIN par3.instrumentounidade inu ON pro.inuid = inu.inuid
         LEFT JOIN par3.adesaoescolaacessivel adm ON adm.inuid = pro.inuid
       WHERE
         pro.adpano_ciclo = {$ano}
         AND pro.pfaid = {$pfaid}
         AND inu.itrid = 2

      UNION
        select
	distinct i.muncod,
            'Não acessou o programa' as situacao_aceitacao_termo,
            'Não acessou o programa' as situacao_adesao,
            '' as id
        from  
            par3.instrumentounidade i
        inner join 
            par3.escolas_ea ea on 
            ea.muncod = i.muncod
        where
            i.itrid = 2
            and ea.eeaapta = true
            and ea.pfaid = {$pfaid}
            and ea.eeaano_ciclo = '{$ano}'
            and i.inuid not in(select p.inuid from par3.prodesaoprograma p where p.pfaid = {$pfaid} and p.adpano_ciclo = {$ano})
     ) AS foo
GROUP BY {$desc};
SQL;*/
    
return <<<SQL
    select
	situacao_aceitacao_termo as descricao,
	count( muncod ) as valor,
	to_json(
		array_agg( muncod )
	) as id
    from
	(
            select
                distinct inu.muncod as muncod,
                case
                    when adpresposta = 'C' then 'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
                    when adpresposta is null then 'Entrou mas não fez registro (não aceitou, nem negou)'
                    when adpresposta = 'S' then 'Aceitou ao termo'
                    when adpresposta = 'N' then 'Não aceitou o termo'
                end as situacao_aceitacao_termo,
                case
                    when adpresposta = 'C' then 'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
                    when adpresposta is null then 'Entrou mas não fez registro (não aceitou, nem negou)'
                    when adpresposta = 'S' then 'Aceitou ao termo'
                    when adpresposta = 'N' then 'Não aceitou o termo'
                end as id,
                esd.esddsc as situacao_adesao
            from
                par3.prodesaoprograma pro
            inner join par3.instrumentounidade inu on
                pro.inuid = inu.inuid
            inner join par3.escolas_ea e1 on
                e1.muncod = inu.muncod
            left join workflow.documento doc on
                doc.docid = pro.docid
            left join workflow.estadodocumento esd on
                esd.esdid = doc.esdid
            left join par3.adesaoescolaacessivel adm on
                adm.inuid = pro.inuid
            where
                pro.adpano_ciclo = {$ano}
                and pro.pfaid = {$pfaid}
                and e1.eeaapta = true
                and e1.eeaselecionada = true 
                and e1.eearede = 'Municipal'
                and (select
                        count(*)
                        from
                            par3.escolas_ea es
                        where
                            es.eeaapta = true
                            and inu.muncod = es.muncod
                            and es.eeaselecionada = true
                            and es.pfaid = 12
                            and es.eeaano_ciclo = '2018'
                            and es.eearede = 'Municipal'
                    ) > 0
                and inu.inuid in(
                    select
                        inuid
                    from
                        (
                            (
                                select 3808 as inuid
                            )
                    union(
                            select
                                distinct iu.inuid
                            from
                                par3.escolas_ea e1
                            inner join par3.instrumentounidade iu on
                                iu.muncod = e1.muncod
                            where
                                e1.eeaapta = true
                                and e1.eeaselecionada = true
                                and e1.pfaid = {$pfaid}
                                and e1.eeaano_ciclo = '{$ano}'
                                and e1.eearede = 'Municipal'
                            group by
                                iu.inuid
                            )
                    ) as inuid
                )
	union 
            select distinct i.muncod,
            'Não acessou o programa' as situacao_aceitacao_termo,
            'Não acessou o programa' as situacao_adesao,
            '' as id
        from
            par3.instrumentounidade i
        inner join par3.escolas_ea ea on
            ea.muncod = i.muncod
        inner join par3.prodesaoprograma pro on
            pro.inuid = i.inuid
        where
            ea.eeaapta = true
            and ea.eeaselecionada = true 
            and ea.pfaid = {$pfaid}
            and ea.eeaano_ciclo = '{$ano}'
            and ea.eearede = 'Municipal'
            and i.inuid not in 
                (select
                    distinct inu.inuid as inuid
                    from
                        par3.prodesaoprograma pro
                    inner join par3.instrumentounidade inu on
                        pro.inuid = inu.inuid
                    inner join par3.escolas_ea e1 on
                        e1.muncod = inu.muncod
                    left join workflow.documento doc on
                        doc.docid = pro.docid
                    left join workflow.estadodocumento esd on
                        esd.esdid = doc.esdid
                    left join par3.adesaoescolaacessivel adm on
                        adm.inuid = pro.inuid
                    where
                        pro.adpano_ciclo = {$ano}
                        and pro.pfaid = {$pfaid}
                        and e1.eeaapta = true
                        and e1.eeaselecionada = true 
                        and e1.eearede = 'Municipal'
                        and inu.inuid in
                            (
                                select 
                                    inuid
                                from
                                    (
                                        (
                                            select 3808 as inuid
                                        )
                                union(
                                    select
                                        distinct iu.inuid
                                    from
                                        par3.escolas_ea e1
                                    inner join par3.instrumentounidade iu on
                                        iu.muncod = e1.muncod
                                    where
                                        e1.eeaapta = true
                                        and e1.eeaselecionada = true
                                        and e1.pfaid = {$pfaid}
                                        and e1.eeaano_ciclo = '{$ano}'
                                        and e1.eearede = 'Municipal'
                                    group by
                                        iu.inuid
                                )
                        ) as inuid
                    )
                ) and (select
                            count(*)
                        from
                            par3.escolas_ea ea
                        where
                            ea.eeaapta = true
                            and i.muncod = ea.muncod
                            and ea.eeaselecionada = true
                            and ea.pfaid = 12
                            and ea.eeaano_ciclo = '2018'
                            and ea.eearede = 'Municipal'
                ) > 0				
            ) as foo
        group by
            situacao_aceitacao_termo;
SQL;

    }
    
    public function getDadosGraficoMunicipioSituacaoAdesao($ano, $pfaid = 12){
        
        return <<<SQL
select
    situacao_adesao as descricao,
    count( muncod ) as valor,
    to_json(
            array_agg( muncod )
    ) as id
from
    (
        select
            distinct inu.muncod as muncod,
            case
                when adpresposta = 'C' then 'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
                when adpresposta is null then 'Entrou mas não fez registro (não aceitou, nem negou)'
                when adpresposta = 'S' then 'Aceitou ao termo'
                when adpresposta = 'N' then 'Não aceitou o termo'
            end as situacao_aceitacao_termo,
            case
                when adpresposta = 'C' then 'Inicialmente negou e depois pediu cancelamento da resposta (retorna para cadastramento)'
                when adpresposta is null then 'Entrou mas não fez registro (não aceitou, nem negou)'
                when adpresposta = 'S' then 'Aceitou ao termo'
                when adpresposta = 'N' then 'Não aceitou o termo'
            end as id,
            esd.esddsc as situacao_adesao
        from
            par3.prodesaoprograma pro
        inner join par3.instrumentounidade inu on
            pro.inuid = inu.inuid
        inner join par3.escolas_ea e1 on
            e1.muncod = inu.muncod
        left join workflow.documento doc on
            doc.docid = pro.docid
        left join workflow.estadodocumento esd on
            esd.esdid = doc.esdid
        left join par3.adesaoescolaacessivel adm on
            adm.inuid = pro.inuid
        where
            pro.adpano_ciclo = {$ano}
            and pro.pfaid = {$pfaid}
            and e1.eeaapta = true
            and e1.eeaselecionada = true 
            and e1.eearede = 'Municipal'
            and (select
                    count(*)
                    from
                        par3.escolas_ea es
                    where
                        es.eeaapta = true
                        and inu.muncod = es.muncod
                        and es.eeaselecionada = true
                        and es.pfaid = 12
                        and es.eeaano_ciclo = '2018'
                        and es.eearede = 'Municipal'
                ) > 0
            and inu.inuid in(
                select
                    inuid
                from
                    (
                        (
                            select 3808 as inuid
                        )
                union(
                    select
                        distinct iu.inuid
                    from
                        par3.escolas_ea e1
                    inner join par3.instrumentounidade iu on
                        iu.muncod = e1.muncod
                    where
                        e1.eeaapta = true
                        and e1.eeaselecionada = true
                        and e1.pfaid = {$pfaid}
                        and e1.eeaano_ciclo = '{$ano}'
                    group by
                        iu.inuid
                    )
                ) as inuid
            )
	union select
                    distinct i.muncod,
                    'Não acessou o programa' as situacao_aceitacao_termo,
                    'Não acessou o programa' as situacao_adesao,
                    case
                        when i.muncod is not null then 'Não acessou o programa'
                    end as id
		from
                    par3.instrumentounidade i
		inner join par3.escolas_ea ea on
                    ea.muncod = i.muncod
		where
                    ea.eeaapta = true
                    and ea.eeaselecionada = true
                    and ea.pfaid = {$pfaid}
                    and ea.eeaano_ciclo = '{$ano}'
                    and i.muncod not in(
                        select
                                distinct inu.muncod as muncod
                        from
                                par3.prodesaoprograma pro
                        inner join par3.instrumentounidade inu on
                                pro.inuid = inu.inuid
                        inner join par3.escolas_ea e1 on
                                e1.muncod = inu.muncod
                        left join workflow.documento doc on
                                doc.docid = pro.docid
                        left join workflow.estadodocumento esd on
                                esd.esdid = doc.esdid
                        left join par3.adesaoescolaacessivel adm on
                                adm.inuid = pro.inuid
                        where
                            pro.adpano_ciclo = {$ano}
                            and pro.pfaid = {$pfaid}
                            and e1.eeaapta = true
                            and e1.eeaselecionada = true 
                            and e1.eearede = 'Municipal'
                            and inu.inuid in(
                                select
                                    inuid
                                from
                                    (
                                        (
                                            select 3808 as inuid
                                        )
                                union(
                                    select
                                        distinct iu.inuid
                                    from
                                        par3.escolas_ea e1
                                    inner join par3.instrumentounidade iu on
                                        iu.muncod = e1.muncod
                                    where
                                        e1.eeaapta = true
                                        and e1.eeaselecionada = true
                                        and e1.pfaid = {$pfaid}
                                        and e1.eeaano_ciclo = '{$ano}'
                                    group by
                                        iu.inuid
                                    )
                            ) as inuid
                        )
                    ) and (select
                            count(*)
                        from
                            par3.escolas_ea ea
                        where
                            ea.eeaapta = true
                            and i.muncod = ea.muncod
                            and ea.eeaselecionada = true
                            and ea.pfaid = 12
                            and ea.eeaano_ciclo = '2018'
                            and ea.eearede = 'Municipal'
                    ) > 0	
            ) as foo
        group by
            situacao_adesao;
SQL;
                            
                  
    }
    
    public function getDadosGraficoUFSaldoDisponivel($ano, $pfaid = 12){
        
        if(!empty($ano) && !empty($pfaid) )
        {
            $sql = "select distinct iu.estuf, iu.inudescricao, ha.hsdasaldo 
                    from par3.adesaoescolaacessivel aes
                    inner join par3.historicosaldodisponiveladesao ha on ha.hsdaestuf = aes.estuf
                    inner join par3.instrumentounidade iu on iu.estuf = ha.hsdaestuf
                    inner join par3.prodesaoprograma pdp on pdp.pfaid = ha.pfaid
                    where ha.pfaid = $pfaid
                    and ha.hsdastatus = 'A'
                    and ha.hsdasaldo > 0
                    and iu.itrid in (1, 3)
                    and ha.hsdamuncod is null  order by ha.hsdasaldo desc;";
            
            //$result = $this->carregar($sql);
            $cabecalho = array('UF', 'Estado', 'Saldo Disponível (R$)');
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setCabecalho($cabecalho);
            $listagem->setQuery($sql);
            $listagem->turnOffForm();
            $listagem->addCallbackDeCampo('hsdasaldo', 'par3_mascaraMoeda');
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagem->render();
        } 
    }
    
    public function getDadosGraficoMunicipioSaldoDisponivel($ano, $pfaid = 12){
        
        if(!empty($ano) && !empty($pfaid) )
        {
            $sql = "select  distinct iu.estuf, iu.inudescricao, ha.hsdasaldo 
                    from par3.adesaoescolaacessivel aes
                    inner join par3.historicosaldodisponiveladesao ha on ha.hsdamuncod = aes.muncod
                    inner join par3.instrumentounidade iu on iu.muncod = ha.hsdamuncod
                    inner join par3.prodesaoprograma pdp on pdp.pfaid = ha.pfaid
                    where ha.pfaid = $pfaid
                    and ha.hsdastatus = 'A'
                    and ha.hsdasaldo > 0
                    and iu.itrid in (2)
                    and ha.hsdaestuf is null order by ha.hsdasaldo desc;";
            
            //$result = $this->carregar($sql);
            
            $cabecalho = array('UF', 'Município', 'Saldo Disponível (R$)');
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setCabecalho($cabecalho);
            $listagem->setQuery($sql);
            $listagem->turnOffForm();
            $listagem->addCallbackDeCampo('hsdasaldo', 'par3_mascaraMoeda');
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagem->render();
            
        } 
    }   
    
    public function getTotalSaldoDisponivel($ano, $pfaid = 12, $tipo){
        
        if(!empty($ano) && !empty($pfaid))
        {
            if($tipo == 'UF'){
               $where = ' and ha.hsdaestuf is not null ';
            } else {
                $where = ' and ha.hsdaestuf is null ';
            }
                $sql = "select
                        SUM(ha.hsdasaldo) as saldodisp
                    from
                        par3.historicosaldodisponiveladesao ha
                    where
                        ha.pfaid = {$pfaid}
                        and ha.hsdastatus = 'A'
                        and ha.hsdasaldo > 0
                        $where ";
            
            return $this->pegaLinha($sql);
        } 
    }   
    
    function trataValorUF($listaUf){  
        if ( !empty($listaUf) && is_array($listaUf) ) {            
            $estuf = "";
            foreach ($listaUf as $key => $value){
                $estuf .= "'". $value . "', ";
            }
            $size = strlen($estuf);
            $estuf = substr($estuf, 0, $size-2); 
            return " AND iu.estuf IN (".$estuf.") ";
        } else {
            return false;
        }
    }
    
    function trataValorMuncod($listaMuncod) {
        if ( !empty($listaMuncod) && is_array($listaMuncod) ) {
            $muncod = "";
            foreach ($listaMuncod as $value){
                $muncod .= "'". $value . "', ";
            }
            $size = strlen($muncod);
            $muncod = substr($muncod,0, $size-2);
            return " AND iu.muncod IN (".$muncod.") ";
        } else {
            return false;
        }        
    }

    function detalheDirigenteTermoUF($resUF, $tipo) {
        
        $uf = $this->trataValorUF($resUF);        
        //$pflcod = ' and pu.pflcod = 1437';
        $tenid = 9;
                
        if ($uf) {            
            $sql = "select
                        distinct 
                        iu.estuf as uf,
                        iu.inudescricao as estado,
                        iue.entnome as nomedirigente,
                        lower( iue.entemail ) as emaildirigente,
                        lower( iue.enttelefonecomercial ) as telefone
                    from
                        seguranca.usuario_sistema us
                    inner join par3.instrumentounidade_entidade iue on
                        iue.entcpf = us.usucpf
                    inner join par3.instrumentounidade iu on
                        iu.inuid = iue.inuid
                    inner join seguranca.perfilusuario pu on
                        pu.usucpf = us.usucpf
                    where
                        us.sisid = 231
                        {$pflcod}
                        and us.susstatus = 'A'
                        -- and us.suscod = 'A' 
                        and iue.tenid in({$tenid})
                        {$uf}
                        and iu.itrid in(1, 3)
                        and iue.entstatus = 'A'
                    order by 1, 2;";
            
            $cabecalho = array('UF', 'Estado', 'Nome', 'Email', 'Telefone');
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setCabecalho($cabecalho);
            $listagem->setQuery($sql);
            $listagem->setId('dirigentetermouf');
            $listagem->turnOffForm();
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagem->render();
            
            return $res;
            
        } else {
            return false;
        }
    }
    
    function detalheDirigenteTermoMunicipio($resMun, $tipo){
        
        $resMunicipio = $this->trataValorMuncod($resMun);        
        //$pflcod = ' and pu.pflcod = 1433';
        $tenid = 4;
                
        if ($resMunicipio) {            
            $sql = "select
                        distinct
                        iu.estuf as uf,
                        iu.inudescricao as estado,
                        trim(u.usunome) as nomedirigente,
                        lower( iue.entemail ) as emaildirigente,
                        lower( iue.enttelefonecomercial ) as telefone
                    from
                        seguranca.usuario_sistema us
                    inner join par3.instrumentounidade_entidade iue on
                        iue.entcpf = us.usucpf
                    inner join seguranca.perfilusuario pu on
                        pu.usucpf = us.usucpf
                    inner join seguranca.usuario u on
                        u.usucpf = us.usucpf
                    inner join par3.instrumentounidade iu on
                        iu.inuid = iue.inuid
                    where
                        us.sisid = 231
                        {$pflcod}
                        and us.susstatus = 'A'
                        -- and us.suscod = 'A'
                        and iue.tenid = {$tenid}
                        and iu.itrid in (2)
                        {$resMunicipio}
                        and iue.entstatus = 'A'
                    order by 1, 2;";
            
            $cabecalho = array('UF', 'Município', 'Nome', 'Email', 'Telefone');
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setCabecalho($cabecalho);
            $listagem->setQuery($sql);
            $listagem->setId('dirigentetermomun');
            $listagem->turnOffForm();
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagem->render();
            
            return $res;
            
        } else {
            return false;
        }
    }
    
    // recupera dados do progrema e total de escolas aptas e selecionadas do programa por ano exercicio 
    function recuperaDadosAdesaoPAR($exercicio) {

        $sql =   "SELECT pfa.pfavalortotalprograma
                            FROM   par3.proadesao pfa
                            INNER JOIN par3.programa prg ON prg.prgid = pfa.prgid
                            INNER JOIN par3.programaorigem po ON po.pgoid = prg.pgoid
                            WHERE prg.prganoreferencia = '{$exercicio}'
                            AND pfa.pfaano = '{$exercicio}' AND po.pgoid = 12 ";
                            
        return $this->pegaLinha($sql);
    }
    
}
