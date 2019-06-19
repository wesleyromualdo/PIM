<?php

class Par3_Model_Adesaoalfabetizacao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.adesaoescolamaisalfabetizacao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'aemid'
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
        'muncod' => array('tabela' => 'territorios.municipio', 'pk' => 'muncod'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'aemid' => null,
        'codinep' => null,
        'inuid' => null,
        'muncod' => null,
        'estuf' => null,
        'aemcpf' => null,
        'aemcargahoraria' => null,
        'aemstatus' => null,
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
            'aemid' => array('Digits'),
            'codinep' => array('allowEmpty' => true, 'Digits'),
            'inuid' => array('allowEmpty' => true, 'Digits'),
            'aemcargahoraria' => array('allowEmpty' => true, 'Digits'),
            'muncod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 7))),
            'estuf' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 2))),
            'aemcpf' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))),
            'aemstatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
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

    public function existeEscolas($inuid, $adpid, $carga_horaria = false)
    {
        $sql_inst = "SELECT itrid FROM par3.instrumentounidade WHERE inuid = {$inuid}";
        $itrid =  $this->pegaLinha($sql_inst);
        $itrid =  ($itrid ? $itrid['itrid']: '');

        if ($itrid == 2) {
            $arrWhere[] = " emarede = 'Municipal' ";
            $join = <<<SQL
INNER JOIN par3.instrumentounidade iu ON iu.muncod = aea.muncod
SQL;
        } elseif ($itrid == 1 || $itrid == 3) {
            $arrWhere[] = " emarede = 'Estadual' ";
            $join = <<<SQL
INNER JOIN par3.instrumentounidade iu ON iu.estuf = aea.estuf
SQL;
        }

        if ($adpid) {
            if ($carga_horaria) {
                $arrWhere[] = " emacargahoraria = {$carga_horaria}";
            }

            $sqlWhere = ' AND ' . implode(' AND ', $arrWhere);


            $sql = <<<SQL
SELECT count(*) AS qtd_registros
  FROM   par3.escolasmaisalfabetizacao   aea
  {$join} 
  WHERE inuid = {$inuid}  {$sqlWhere} 
SQL;
            $result = $this->pegaUm($sql);
        }
        return ($result != '') ? true : false;
    }

    public function retornaQtdSalvoGrupo($dados)
    {
        $inuid = $dados['inuid'];
        $cargahoraria = $dados['carga_horaria'];
        $adpid = $dados['adpid'];

        if ($adpid) {
            $sql = "
        		SELECT 
    				count(aemid) 
    			FROM 
    				par3.adesaoescolamaisalfabetizacao
    			WHERE 
    			    aemcargahoraria = {$cargahoraria} AND
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
    			UPDATE par3.adesaoescolamaisalfabetizacao SET 
	    			aemcargahoraria = NULL
	    		WHERE
	    			inuid = '{$inuid}' AND
                    adpid = $adpid AND
	    			aemgrupo = '{$grupo}'";

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
        if($dados['aeacargahoraria'] != 'N')
            $aemcargahoraria = " aemcargahoraria = {$dados['aeacargahoraria']} AND " ;
        else
            $aemcargahoraria = '';
        if ($adpid) {
            $sql = "SELECT 
                		DISTINCT codinep
                	FROM
                		par3.adesaoescolamaisalfabetizacao
                	WHERE 1 = 1 AND
                		{$aemcargahoraria}
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

    /**
     *
     * */
    public function salvaFormularioEscolas($arrParametros)
    {

        // Objetos utilizados
        $objIstrumentoUnidade = new Par3_Model_InstrumentoUnidade();

        // Dados enviados por parametro
        $inuid = $arrParametros['inuid'];
        $adpid = $arrParametros['adpid'];
        $cpf = $_SESSION['usucpf'];

        $docid = $this->pegaUm("SELECT docid FROM par3.prodesaoprograma WHERE adpid = $adpid");

        $enviadoApos = FALSE;
        if ($docid) {
            $situacaoAtual = wf_pegarEstadoAtual($docid);
            if (($situacaoAtual['esdid'] == WF_ESDID_ENVIADOPARAOMEC_MAISALFABETIZACAO)) {
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
        $carga_horaria =  $arrParametros['carga_horaria'];

        /**********************************************/

        if ((!$enviadoApos)) {
            $this->executar("DELETE FROM  par3.adesaoescolamaisalfabetizacao WHERE  adpid = $adpid AND inuid = {$inuid} AND aemcargahoraria = {$carga_horaria} ");
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

                $arrDadosSalvar['codinep'] = $codInep;
                $arrDadosSalvar['inuid'] = $inuid;
                $arrDadosSalvar['adpid'] = $adpid;
                $arrDadosSalvar['estuf'] = $estuf;
                $arrDadosSalvar['aemcpf'] = $cpf;
                $arrDadosSalvar['aemcargahoraria'] = $arrParametros['carga_horaria'];
                $arrDadosSalvar['aeminseridaaposenvio'] = $enviadoApos;

                if ($itrid == 2) {
                    $arrDadosSalvar['muncod'] = $muncod;
                }
                $salvo = $this->insereEscola($arrDadosSalvar);

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

    function insereEscola($arrDadosSalvar)
    {
        $codinep = $arrDadosSalvar['codinep'];
        $inuid = $arrDadosSalvar['inuid'];
        $adpid = $arrDadosSalvar['adpid'];
        $estuf = $arrDadosSalvar['estuf'];
        $aemcpf = $arrDadosSalvar['aemcpf'];
        $muncod = ($arrDadosSalvar['muncod']) ? "'{$arrDadosSalvar['muncod']}'" : 'NULL';
        $aemcargahoraria = ($arrDadosSalvar['aemcargahoraria']) ? $arrDadosSalvar['aemcargahoraria'] : 'NULL';
        $aeminseridaaposenvio = ($arrDadosSalvar['aeminseridaaposenvio']) ? 'TRUE' : 'FALSE';

        $sqlInsert = "
    		INSERT INTO 
				par3.adesaoescolamaisalfabetizacao (codinep, inuid, adpid, muncod, estuf, aemcpf, aemcargahoraria, aeminseridaaposenvio )
			VALUES
                ({$codinep}, {$inuid}, $adpid, {$muncod}, '{$estuf}','{$aemcpf}',$aemcargahoraria, $aeminseridaaposenvio )		
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

    public function listaAdesoesProemiAbcMais($ano, $pfaid = 10)
    {
        $sql = <<<SQL
SELECT
  *,
  CASE
  WHEN esfera = 'Estadual'
    THEN
      (SELECT count(*)
       FROM par3.escolasmaisalfabetizacao
       WHERE emacargahoraria = 10 AND emarede = esfera AND estuf = foo.uf
       GROUP BY estuf)
  WHEN esfera = 'Municipal'
    THEN
      (SELECT count(*)
       FROM par3.escolasmaisalfabetizacao
       WHERE emacargahoraria = 10 AND emarede = esfera AND muncod = cod_municipio
       GROUP BY muncod)
  END AS total_escolas_10,
  CASE
  WHEN esfera = 'Estadual'
    THEN
      (SELECT count(*)
       FROM par3.escolasmaisalfabetizacao
       WHERE emacargahoraria = 5 AND emarede = esfera AND estuf = foo.uf
       GROUP BY estuf)

  WHEN esfera = 'Municipal'
    THEN
      (SELECT count(*)
       FROM par3.escolasmaisalfabetizacao
       WHERE emacargahoraria = 5 AND emarede = esfera AND muncod = cod_municipio
       GROUP BY muncod)
  END AS total_escolas_5,

  CASE WHEN adpid IS NULL
    THEN 0
  ELSE

    (SELECT count(*)
     FROM par3.adesaoescolamaisalfabetizacao
     WHERE aemcargahoraria = 10 AND inuid = foo.inuid AND adpid = foo.adpid)
  END AS total_escolas_selecionadas_10,
  CASE WHEN adpid IS NULL
    THEN 0
  ELSE
    (SELECT count(*)
     FROM par3.adesaoescolamaisalfabetizacao
     WHERE aemcargahoraria = 5 AND inuid = foo.inuid AND adpid = foo.adpid)
  END AS total_escolas_selecionadas_5


FROM (
       SELECT
         DISTINCT
         'MAIS ALFABETIZAÇÃO' AS programa,
         inu.estuf            AS uf,
         inu.muncod           AS cod_municipio,
         inu.inudescricao     AS municipio_descricao,
         CASE WHEN inu.itrid IN (1, 3)
           THEN
             'Estadual'
         ELSE
           'Municipal'
         END                  AS esfera,
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
         END                  AS situacao_aceitacao_termo,
         esd.esddsc           AS situacao_adesao,
         inu.inuid,
         pro.adpid
       FROM par3.prodesaoprograma pro
         LEFT JOIN workflow.documento doc ON doc.docid = pro.docid
         LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
         LEFT JOIN par3.instrumentounidade inu ON pro.inuid = inu.inuid
         LEFT JOIN par3.adesaoescolamaisalfabetizacao adm ON adm.inuid = pro.inuid
       WHERE
         adpano_ciclo = {$ano}
         AND pfaid = {$pfaid}

       UNION

       SELECT
         'MAIS ALFABETIZAÇÃO'                                   AS programa,
         inu.estuf                                              AS uf,
         inu.muncod                                             AS cod_municipio,
         inu.inudescricao                                       AS municipio_descricao,
         CASE WHEN inu.itrid IN (1, 3)
           THEN
             'Estadual'
         ELSE
           'Municipal'
         END                                                    AS esfera,
         'Não acessou o programa' AS situacao_aceitacao_termo,
         'Não acessou o programa'                               AS situacao_adesao,
         inuid,
         NULL
       FROM par3.instrumentounidade inu
       WHERE inuid NOT IN (SELECT inuid
                           FROM par3.prodesaoprograma
                           WHERE pfaid = {$pfaid} AND adpano_ciclo = {$ano})
     ) AS foo;

SQL;

        $result = $this->carregar($sql);
        $result = (is_array($result)) ? $result : array();
        return  (count($result) > 0) ? $result : false;

    }

    public function getDadosGraficoUfSituacao($ano, $pfaid=10, $desc = 'situacao_aceitacao_termo'){
        return <<<SQL
SELECT
   {$desc}  AS DESCRICAO,
  COUNT(uf) AS VALOR

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
         esd.esddsc AS situacao_adesao

       FROM par3.prodesaoprograma pro
         LEFT JOIN workflow.documento doc ON doc.docid = pro.docid
         LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
         LEFT JOIN par3.instrumentounidade inu ON pro.inuid = inu.inuid
         LEFT JOIN par3.adesaoescolamaisalfabetizacao adm ON adm.inuid = pro.inuid
       WHERE
         pro.adpano_ciclo = {$ano}
         AND pro.pfaid = {$pfaid}
         AND inu.itrid IN (1, 3)

      UNION

         SELECT
           DISTINCT estuf,
           'Não acessou o programa' AS situacao_aceitacao_termo,
           'Não acessou o programa' AS situacao_adesao
         FROM par3.instrumentounidade
        WHERE inuid NOT IN (SELECT inuid FROM par3.prodesaoprograma WHERE pfaid = 10 AND  adpano_ciclo = {$ano} )
        AND itrid IN (1, 3)
     ) AS foo
  GROUP BY {$desc}
SQL;
    }

    public function getDadosGraficoMunicipioSituacao($ano, $pfaid=10, $desc = 'situacao_aceitacao_termo'){
        return <<<SQL
SELECT
  {$desc}  AS DESCRICAO,
  COUNT(muncod) AS VALOR

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
         esd.esddsc AS situacao_adesao

       FROM par3.prodesaoprograma pro
         LEFT JOIN workflow.documento doc ON doc.docid = pro.docid
         LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
         LEFT JOIN par3.instrumentounidade inu ON pro.inuid = inu.inuid
         LEFT JOIN par3.adesaoescolamaisalfabetizacao adm ON adm.inuid = pro.inuid
       WHERE
         pro.adpano_ciclo = {$ano}
         AND pro.pfaid = {$pfaid}
         AND inu.itrid = 2

      UNION

         SELECT
           DISTINCT muncod,
           'Não acessou o programa' AS situacao_aceitacao_termo,
           'Não acessou o programa' AS situacao_adesao
         FROM par3.instrumentounidade
        WHERE inuid NOT IN (SELECT inuid FROM par3.prodesaoprograma WHERE pfaid = {$pfaid} AND  adpano_ciclo = {$ano} )
        AND itrid = 2
     ) AS foo
GROUP BY {$desc};
SQL;
    }
}
