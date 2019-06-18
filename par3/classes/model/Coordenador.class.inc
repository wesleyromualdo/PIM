<?php

/**
 * Classe de mapeamento da entidade par3.contratos.
 *
 * @version $Id$
 * @since 2017.03.02
 */

/**
 * Par3_Model_Contratos: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardo.neto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Contratos($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Contratos($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $conid chave primária do contrato - default: nextval('par3.contratos_conid_seq'::regclass)
 * @property int $inuid fk da unidade
 * @property int $prgid fk do programa
 * @property numeric $connumero número do contrato
 * @property string $constatus
 * @property \Datetime(Y-m-d H:i:s) $condtcriacao  - default: now()
 * @property string $usucpf
 */

class Par3_Model_Coordenador extends Modelo
{

    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.coordenador';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'cooid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpfinclusao' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'cooid' => null,
        'coonome' => null,
        'coodtainclusao' => null,
        'arqid' => null,
        'coobservacao' => null,
        'usucpfinclusao' => null,
        'coostatus' => null,
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
            'cooid' => array('Digits'),
            'coonome' => array('allowEmpty' => false, new Zend_Validate_StringLength(array('max' => 250))),
            'coodtainclusao' => array('allowEmpty' => true, 'Digits'),
            'arqid' => array('allowEmpty' => false),
            'coobservacao' => array('allowEmpty' => true),
            'usucpfinclusao' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))),
            'coostatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
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
    public
    function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui
        if($this->coodtainclusao == ''){
            $this->coodtainclusao = null;
        }
        if($this->coobservacao == ''){
            $this->coobservacao = ' ';
        }

        $this->usucpfinclusao = $_SESSION['usucpf'];
        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function recuperarPorId($cooid){
        $sql = "SELECT cooid, 
                       coonome,
                       to_char(coodtainclusao, 'DD/MM/YYYY') as coodtainclusao,
                       coobservacao, 
                       usucpfinclusao, 
                       coostatus, 
                       arqid, 
                       arqnome, 
                       arqextensao
                 FROM par3.coordenador
                 INNER JOIN public.arquivo USING (arqid)
                 WHERE cooid = " . $cooid;
        return $this->pegaLinha($sql);
    }


    public function montaSQLLista($dados = array())
    {
        $where = array();

        if (strlen($dados['cooid']) > 0){
            $where[] = ' cooid = '.$dados['cooid'];
        }

        if (strlen($dados['coonome']) > 0){
            $where[] = " coonome ILIKE '%".trim($dados['coonome'])."%' ";
        }

        $whe = ($where ? 'WHERE ' : '').implode(' AND ', $where);

        $ativo = "<b class=\"green-text text-darken-2\">Ativo</b>";
        $inativo = "<b class=\"red-text text-darken-2\">Inativo</b>";

        $sql = "SELECT 
                        cooid as id,
						cooid as codigo,
						coonome,
						arq.arqid, --esconder
						arqnome,
						coobservacao,
						usunome || ' - ' || usucpfinclusao,
						to_char(coodtainclusao, 'DD/MM/YYYY') as coodtainclusao,
						CASE WHEN coostatus = 'A'
						     THEN '$ativo'
						     ELSE '$inativo'
					    END AS coostatus
					FROM par3.coordenador as coo
					INNER JOIN public.arquivo as arq ON arq.arqid = coo.arqid
					LEFT JOIN seguranca.usuario as usu on usu.usucpf = coo.usucpfinclusao
					{$whe}
                    ORDER BY coostatus, cooid";

        return $sql;
    }

    public function existeAtivo(){
        $sql = "SELECT count(cooid) FROM par3.coordenador WHERE coostatus = 'A'";
        return $this->pegaUm($sql);
    }

    public function existeNomeCoordenador($nome, $id = null){
        //caso ao editar, seja informado outro nome já existente
        if($id > 0){
            $_andId = " AND cooid <> " . $id;
        }

        $sql = "SELECT count(cooid) FROM par3.coordenador WHERE coonome = '{$nome}'{$_andId}";
        return $this->pegaUm($sql);
    }


}