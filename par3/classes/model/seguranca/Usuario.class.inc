<?php
/**
 * Classe de mapeamento da entidade seguranca.usuario.
 *
 * @version $Id$
 * @since 2016.08.31
 */

/**
 * Seguranca_Model_Usuario: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author  <>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Seguranca_Model_Usuario($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Seguranca_Model_Usuario($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $usucpf
 * @property string $regcod
 * @property string $usunome
 * @property string $usuemail
 * @property string $usustatus
 * @property string $usufoneddd
 * @property string $usufonenum
 * @property string $ususenha
 * @property \Datetime(Y-m-d H:i:s) $usudataultacesso
 * @property int $usunivel
 * @property string $usufuncao
 * @property string $ususexo
 * @property string $orgcod
 * @property string $unicod
 * @property bool $usuchaveativacao
 * @property smallint $usutentativas
 * @property string $usuprgproposto
 * @property string $usuacaproposto
 * @property string $usuobs
 * @property string $ungcod
 * @property \Datetime(Y-m-d H:i:s) $usudatainc  - default: ('now'::text)::timestamp(6) without time zone
 * @property bool $usuconectado  - default: false
 * @property int $pflcod
 * @property string $suscod  - default: 'A'::bpchar
 * @property string $usunomeguerra
 * @property string $orgao
 * @property string $muncod
 * @property \Datetime(Y-m-d H:i:s) $usudatanascimento
 * @property \Datetime(Y-m-d H:i:s) $usudataatualizacao  - default: now()
 * @property int $entid
 * @property string $tpocod
 * @property int $carid
 */
class Seguranca_Model_Seguranca_Usuario extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.usuario';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'usucpf',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'entid' => array('tabela' => 'entidade.entidade', 'pk' => 'entid'),
        'carid' => array('tabela' => 'cargo', 'pk' => 'carid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array('usucpf' => null, 'regcod' => null, 'usunome' => null, 'usuemail' => null, 'usustatus' => null, 'usufoneddd' => null, 'usufonenum' => null, 'ususenha' => null, 'usudataultacesso' => null, 'usunivel' => null, 'usufuncao' => null, 'ususexo' => null, 'orgcod' => null, 'unicod' => null, 'usuchaveativacao' => null, 'usutentativas' => null, 'usuprgproposto' => null, 'usuacaproposto' => null, 'usuobs' => null, 'ungcod' => null, 'usudatainc' => null, 'usuconectado' => null, 'pflcod' => null, 'suscod' => null, 'usunomeguerra' => null, 'orgao' => null, 'muncod' => null, 'usudatanascimento' => null, 'usudataatualizacao' => null, 'entid' => null, 'tpocod' => null, 'carid' => null,);

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'regcod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 2))),
            'usunome' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'usuemail' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'usustatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'usufoneddd' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 2))),
            'usufonenum' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 10))),
            'ususenha' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'usudataultacesso' => array('allowEmpty' => true),
            'usunivel' => array('allowEmpty' => true, 'Digits'),
            'usufuncao' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 100))),
            'ususexo' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'orgcod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 5))),
            'unicod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 5))),
            'usuchaveativacao' => array('allowEmpty' => true),
            'usutentativas' => array('allowEmpty' => true),
            'usuprgproposto' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1000))),
            'usuacaproposto' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1000))),
            'usuobs' => array('allowEmpty' => true),
            'ungcod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 6))),
            'usudatainc' => array('allowEmpty' => true),
            'usuconectado' => array('allowEmpty' => true),
            'pflcod' => array('allowEmpty' => true, 'Digits'),
            'suscod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'usunomeguerra' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 20))),
            'orgao' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 100))),
            'muncod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 7))),
            'usudatanascimento' => array('allowEmpty' => true),
            'usudataatualizacao' => array('allowEmpty' => true),
            'entid' => array('allowEmpty' => true, 'Digits'),
            'tpocod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'carid' => array('allowEmpty' => true, 'Digits'),
        );
    }
    
    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getNome($usucpf)
    {
    	if($usucpf)
    	{
    		return $this->pegaUm("SELECT usunome FROM seguranca.usuario where usucpf = '{$usucpf}'");
    	}
    	else
    	{
    		return '';
    	}
    	  
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

    function salvarUsuario($dados,$usustatus = null)
    {
        $this->usucpf = addslashes($dados['usucpf']);
        $this->usunome = addslashes($dados['usunome'][0]);
        $this->usuemail = addslashes($dados['usuemail'][0]);
        $this->ususenha = md5_encrypt_senha($dados['ususenha'], '');

        $uf_conselho = $dados['estuf'] = $dados['estuf'][0];
        $co_mun_ibge_conselho =$dados['muncod'] = $dados['muncod'][0];

  
        $usustatus = $usustatus ? $usustatus : 'P';
        $sql = "    INSERT INTO seguranca.usuario ( usucpf, usunome, usuemail, ususenha, usuchaveativacao, suscod, usustatus, regcod, muncod ) 
                    values ( '{$this->usucpf}', '{$this->usunome}', '{$this->usuemail}', '{$this->ususenha}', 'f', 'A', '{$usustatus}', '{$uf_conselho}', '{$co_mun_ibge_conselho}' ) 
                    returning usucpf
                ";
        return $this->executar($sql, false);
    }
    
    
    function verificaExistencia($usucpf)
    {
    	$resultUsuario = $this->carregar ("select usucpf from  seguranca.usuario where usucpf = '{$usucpf}' ");
    	
    	return ($resultUsuario) ? true : false ;
    }


    public function getComboUfs()
    {
        $sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
        $dados = $this->carregar($sql);
        return $this->getOptions($dados, array('prompt' => ' Selecione '), $this->regcod);
    }

    public function getComboMunicipios($estuf)
    {
        $sql = "SELECT muncod AS codigo, mundescricao AS descricao  FROM territorios.municipio WHERE estuf = '" . $estuf . "' ORDER BY descricao";
        $dados = $this->carregar($sql);
        return $this->getOptions($dados, array('prompt' => ' Selecione '), $this->muncod );
    }

    public function getComboTipoOrgao()
    {
        $sql = "SELECT tpocod as codigo, tpodsc as descricao FROM public.tipoorgao WHERE tpostatus='A' ";
        $dados = $this->carregar($sql);
        return $this->getOptions($dados, array('prompt' => ' Selecione '), 'tpocod');
    }

    public function getComboOrgaos($tpocod, $regcod, $muncod)
    {
        $inner = ($tpocod == 3 || $tpocod == 2) ? ' INNER JOIN entidade.endereco eed ON eed.entid = ee.entid ' : '';
        $uniao = ($tpocod == 3 || $tpocod == 2) ? " UNION ALL ( SELECT 999999 AS codigo, 'OUTROS' AS descricao )" : '';

        if ($tpocod == 2) {
            $clausula = " AND eed.estuf = '{$regcod}' ";
        } elseif ($tpocod == 3) {
            $clausula = " AND eed.muncod = '{$muncod}' ";
        }

        $sql = "(SELECT
                        ee.entid AS codigo,
                        CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome
                        ELSE ee.entnome END AS descricao
                FROM
                        entidade.entidade ee
                INNER JOIN entidade.funcaoentidade ef ON ef.entid = ee.entid
                INNER JOIN public.tipoorgaofuncao tpf ON ef.funid = tpf.funid
                        " . $inner . "
                WHERE
                    ee.entstatus = 'A' and
                        tpf.tpocod = '{$tpocod}'
                        " . $clausula . " AND
                        ( ee.entorgcod is null or ee.entorgcod <> '73000' )

                ORDER BY
                        ee.entnome)" . $uniao;
        $dados = $this->carregar($sql);
        if (is_array($dados)){
            return $this->getOptions($dados, array('prompt' => ' Selecione '), 'entid');
        }
        return '';

    }

    public function getOptions(array $dados, array $htmlOptions = array(), $idCampo = null, $descricaoCampo = null)
    {
        $html = '';
        $selected = '';


        if (isset($htmlOptions['prompt'])) {
            $html .= '<option value="">' . strtr($htmlOptions['prompt'], array('<' => '&lt;', '>' => '&gt;')) . "</option>\n";
        }

        if ($dados) {
            foreach ($dados as $data) {
                if ($idCampo) {
                    $selected = ($data['codigo'] === $idCampo ? "selected='true' " : "");
                }
                $html .= "<option {$selected}  title=\"{$data['descricao']}\" value= " . $data['codigo'] . ">  " . simec_htmlentities($data['descricao']) . " </option> ";
            }
        }
        return $html;
    }

    public function inativarUsuarios(array $usuarios)
    {
        try{
            foreach ($usuarios as $cpf) {
                $sql = "UPDATE seguranca.usuario SET suscod = 'B' WHERE usucpf='{$cpf['usucpf']}';";
                $this->executar($sql);
                $this->commit();
            }
        }catch (Exception $exc){
            $this->rollback();
            echo $exc->getTraceAsString();
            return false;
        }
        return true;
    }
    
    public function inserirUsuario($dados)
    {
        $data_atual = date("Y-m-d H:i:s");
        $dataBanco = formata_data_sql($dados['usudatanascimento']);
        $dataBanco = $dataBanco ? "'" . $dataBanco . "'" : "null";

        $novasenha_ = md5_encrypt_senha($dados['novasenha'], '');
        $sql = sprintf(" UPDATE seguranca.usuario SET
                                    usuchaveativacao = 't', 
                                    ususenha='%s',
                                    usuemail = '%s',
                                    usufoneddd = '%s',
                                    usufonenum = '%s',
                                    regcod = '%s',
                                    ususexo = '%s',
                                    usudatanascimento = %s,
                                    muncod = '%s',
                                    usudataatualizacao ='%s'
                                WHERE
                                    usucpf = '%s'",
            pg_escape_string($novasenha_),
            pg_escape_string($dados['usuemail']),
            pg_escape_string($dados['usufoneddd']),
            str_replace("\\", "", substr($dados['usufonenum'], 0, 10)),
            pg_escape_string($dados['regcod']),
            pg_escape_string($dados['ususexo']),
            $dataBanco,
            pg_escape_string($dados['muncod']),
            $data_atual,
            pg_escape_string($_SESSION['usucpf'])
        );

        $saida = $this->executar($sql);
        $this->commit();
        return $saida;
    }

    /*
     *
     */
    public function getFone($usucpf){
        if($usucpf)
        {
            return $this->pegaUm("SELECT usufonenum FROM seguranca.usuario where usucpf = '{$usucpf}'");
        }
        else
        {
            return '';
        }

    }

    /*
     *
     */
    public function getMuncod($usucpf){
        if($usucpf)
        {
            return $this->pegaUm("SELECT muncod FROM seguranca.usuario where usucpf = '{$usucpf}'");
        }
        else
        {
            return '';
        }

    }

    public function getRegcod($usucpf){
        if($usucpf)
        {
            return $this->pegaUm("SELECT regcod FROM seguranca.usuario where usucpf = '{$usucpf}'");
        }
        else
        {
            return '';
        }

    }

    public function getFoneDDD($usucpf){
        if($usucpf)
        {
            return $this->pegaUm("SELECT usufoneddd FROM seguranca.usuario where usucpf = '{$usucpf}'");
        }
        else
        {
            return '';
        }

    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description esta função retorna os perfis técnicos que analisa as obras
     * @return array
     */
    public function perfisTecnicosFiltroPar3Obras(){
        $arrPerfis = array(PAR3_PERFIL_SUPER_USUARIO,PAR3_PERFIL_ADMINISTRADOR,PAR3_PERFIL_PREFEITO);
        return $arrPerfis;
    }

}//end class
