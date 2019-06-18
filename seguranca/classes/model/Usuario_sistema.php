<?php
/**
 * Classe de mapeamento da entidade seguranca.usuario_sistema.
 *
 * @version $Id$
 * @since 2017.02.03
 */
 
/**
 * Seguranca_Model_Usuario_sistema: sem descricao
 *
 * @package Seguranca\Model
 * @uses Simec\Db\Modelo
 * @author Victor Martins Machado <victormachado@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Seguranca_Model_Usuario_sistema($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Seguranca_Model_Usuario_sistema($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $suscod  - default: 'A'::bpchar
 * @property \Datetime(Y-m-d H:i:s) $susdataultacesso  - default: now()
 * @property int $pflcod 
 * @property string $susstatus  - default: 'A'::bpchar
 * @property int $sisid 
 * @property string $usucpf
 */
class Seguranca_Model_Usuario_sistema extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.usuario_sistema';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'usucpf',
        'sisid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'sisid' => array('tabela' => 'seguranca.sistema', 'pk' => 'sisid'),
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'suscod' => null,
        'susdataultacesso' => null,
        'pflcod' => null,
        'susstatus' => null,
        'sisid' => null,
        'usucpf' => null
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
            'suscod' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'susdataultacesso' => array('allowEmpty' => true),
            'pflcod' => array('allowEmpty' => true,'Digits'),
            'susstatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'sisid' => array('Digits'),
            'sisid' => array('Digits'),
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
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

    public function carregarPorId($id){
        if(is_numeric($id)){
            $id = trim( str_replace("'", "",(string)$id) );
            $sql = " SELECT * FROM $this->stNomeTabela WHERE {$this->arChavePrimaria[0]} = '$id'; ";

            $arResultado = $this->pegaLinha( $sql );
            $this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
        } else {
            if(is_array($id)){
                $where = "";
                foreach ($this->arChavePrimaria as $chave) {
                    $and = !empty($where) ? 'AND' : '';
                    $where .= " {$and} {$chave} = '{$id[$chave]}'";
                }
                $sql = <<<DML
                    SELECT * FROM {$this->stNomeTabela} WHERE {$where}
DML;

                $arResultado = $this->pegaLinha( $sql );
                $this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
            }
        }
    }

    public function validaChavePrimaria(){
        if(!is_array($this->arChavePrimaria)){
            foreach ($this->arChavePrimaria as $chave) {
                if(empty($this->$chave)){
                    return false;
                }
            }
            return true;
        } else {
            $stChavePrimaria = $this->arChavePrimaria[0];
            return !empty($this->$stChavePrimaria) ? true : false;
        }
    }

    public function retornaChavePrimaria(){
        $retorno = '';
        if(count($this->arChavePrimaria) > 1){
            $retorno = [];
            foreach ($this->arChavePrimaria as $chave) {
                $retorno[$chave] = $this->$chave;
            }
        } else {
            $stChavePrimaria = $this->arChavePrimaria[0];
            $retorno = $this->$stChavePrimaria;
        }
        return $retorno;
    }

    public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array(), $manterAspas = false){
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();

        if( $boAntesSalvar ){
            if( !$this->antesSalvar() ){ return false; }
        }

        if( $this->validaChavePrimaria() && !$this->tabelaAssociativa ){
            $this->alterar($arCamposNulo);
            $resultado = $this->retornaChavePrimaria();
        }else{
            if($manterAspas === false){
                $resultado = $this->inserir($arCamposNulo);
            }else{
                $resultado = $this->inserirManterAspas($arCamposNulo);
            }
        }
        if( $resultado ){
            if( $boDepoisSalvar ){
                $this->depoisSalvar();
            }
        }
        return $resultado;
    }

    public function inserir($arCamposNulo = array())
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
//        if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );
        $arCampos  = array();
        $arValores = array();
        $arSimbolos = array();

        $troca = array("'", "\\");
        foreach( $this->arAtributos as $campo => $valor ){
//            if( $campo == $this->arChavePrimaria[0] && !$this->tabelaAssociativa ) continue;
            if( $valor !== null ){
                if( !$valor && in_array($campo, $arCamposNulo) ){ continue; }
                $arCampos[]  = $campo;
                $valor = str_replace($troca, "", $valor);
                $arValores[] = trim( pg_escape_string( $valor ) );
            }
        }

        if( count( $arValores ) ){
            $sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." )
											  values ( '". implode( "', '", $arValores ) ."' )
					 returning {$this->arChavePrimaria[0]}";
            $stChavePrimaria = $this->arChavePrimaria[0];
            return $this->$stChavePrimaria = $this->pegaUm( $sql );
        }
    }

    public function alterar($arCamposNulo = array())
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
//        if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );

        $campos = "";
        foreach( $this->arAtributos as $campo => $valor ){
            if( $valor != null ){
                if( in_array($campo, $this->arChavePrimaria) ){
                    continue;
                }

                $valor = pg_escape_string( $valor );

                $campos .= $campo." = '".$valor."', ";
            }
            else{
                if(in_array($campo, $arCamposNulo)) {
                    $campos .= $campo." = null, ";
                }
            }
        }

        $campos = substr( $campos, 0, -2 );

        $where = "";
        foreach ($this->arChavePrimaria as $chave) {
            $and = !empty($where) ? 'AND' : '';
            $where .= " {$and} {$chave} = '{$this->arAtributos[$chave]}'";
        }

        if(!empty($campos) && $this->arChavePrimaria[0]){
            $sql = " UPDATE $this->stNomeTabela SET $campos WHERE {$where} ";
            return $this->executar( $sql );
        } else {
            return false;
        }
    }

}
